#include "@/public/core.h"

#include "@/spinlock.h"

#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <uthash/uthash.h>

struct YalogLogger {
  volatile int threshold;
  YalogSpinlock lock;
  YalogSink* sink;
  UT_hash_handle hh;
  char category[1];
};

static pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;
static YalogLogger default_logger_storage = {.threshold = INT_MAX,
                                             .lock = YALOG_SPINLOCK_INIT};
static YalogLogger* global_loggers = NULL;
static const YalogConfig* global_config = NULL;

YalogLogger* const yalog_default_logger = &default_logger_storage;

static void YalogLoggerResetSink(YalogLogger* logger) {
  YalogSink* sink = NULL;
  if (global_config) {
    sink = global_config->GetSink(global_config, logger->category);
  }
  int threshold = INT_MAX;
  if (sink) {
    threshold = sink->threshold;
  }
  __atomic_store_n(&logger->threshold, threshold, __ATOMIC_RELAXED);
  YalogSpinLock(&logger->lock);
  YalogSink* old_sink = logger->sink;
  logger->sink = sink;
  YalogSpinUnlock(&logger->lock);
  YALOG_REF_RELEASE(old_sink);
}

YalogLogger* YalogGetLogger(const char* category) {
  YalogLogger* logger;
  pthread_mutex_lock(&global_mutex);
  if (*category == '\0') {
    pthread_mutex_unlock(&global_mutex);
    return yalog_default_logger;
  }
  HASH_FIND_STR(global_loggers, category, logger);
  if (logger) {
    pthread_mutex_unlock(&global_mutex);
    return logger;
  }
  logger = malloc(sizeof(YalogLogger) + strlen(category));
  strcpy(logger->category, category);
  YalogSpinInit(&logger->lock);
  logger->sink = NULL;
  HASH_ADD_STR(global_loggers, category, logger);
  YalogLoggerResetSink(logger);
  pthread_mutex_unlock(&global_mutex);
  return logger;
}

const char* YalogLoggerGetCategory(YalogLogger* logger) {
  return logger->category;
}

void YalogLoggerSend(YalogLogger* logger, const YalogMessage* message) {
  YalogSink* sink = NULL;
  YalogSpinLock(&logger->lock);
  if (logger->sink && logger->sink->threshold <= message->severity) {
    sink = YALOG_REF_ACQUIRE(logger->sink);
  }
  YalogSpinUnlock(&logger->lock);
  if (sink) {
    sink->Send(sink, message);
    YALOG_REF_RELEASE(sink);
  }
}

void YalogSetConfig(const YalogConfig* config) {
  pthread_mutex_lock(&global_mutex);
  const YalogConfig* old_config = global_config;
  global_config = config;
  YalogLoggerResetSink(yalog_default_logger);
  for (YalogLogger* logger = global_loggers; logger; logger = logger->hh.next) {
    YalogLoggerResetSink(logger);
  }
  pthread_mutex_unlock(&global_mutex);
  YALOG_REF_RELEASE(old_config);
}
