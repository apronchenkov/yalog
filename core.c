#include "public/core.h"
#include "spinlock.h"
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

struct YalogLogger {
  volatile int threshold;
  YalogSpinlock lock;
  YalogSink *sink;
  YalogLogger *next;
  char category[1];
};

static pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;
static YalogLogger default_logger_storage = {.lock = YALOG_SPINLOCK_INIT};
static YalogLogger *global_loggers_head = &default_logger_storage;
static const YalogConfig *global_config = NULL;

YalogLogger *const default_logger = &default_logger_storage;

static void YalogLoggerResetSink(YalogLogger *logger) {
  YalogSink *sink = NULL;
  if (global_config) {
    sink = global_config->GetSink(global_config, logger->category);
  }
  int threshold = INT_MAX;
  if (sink) {
    threshold = sink->threshold;
  }
  __atomic_store_n(&logger->threshold, threshold, __ATOMIC_RELAXED);
  YalogSpinLock(&logger->lock);
  YalogSink *old_sink = logger->sink;
  logger->sink = sink;
  YalogSpinUnlock(&logger->lock);
  YALOG_REF_RELEASE(old_sink);
}

YalogLogger *YalogGetLogger(const char *category) {
  YalogLogger *logger;
  pthread_mutex_lock(&global_mutex);
  for (logger = global_loggers_head; logger; logger = logger->next) {
    if (0 == strcmp(logger->category, category)) {
      pthread_mutex_unlock(&global_mutex);
      return logger;
    }
  }
  logger = malloc(sizeof(YalogLogger) + strlen(category));
  strcpy(logger->category, category);
  YalogSpinInit(&logger->lock);
  logger->next = global_loggers_head;
  global_loggers_head = logger;
  logger->sink = NULL;
  YalogLoggerResetSink(logger);
  pthread_mutex_unlock(&global_mutex);
  return logger;
}

const char *YalogLoggerGetCategory(YalogLogger *logger) {
  return logger->category;
}

void YalogLoggerSend(YalogLogger *logger, const YalogMessage *message) {
  YalogSink *sink = NULL;
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

void YalogSetConfig(const YalogConfig *config) {
  pthread_mutex_lock(&global_mutex);
  const YalogConfig *old_config = global_config;
  global_config = config;
  for (YalogLogger *logger = global_loggers_head; logger;
       logger = logger->next) {
    YalogLoggerResetSink(logger);
  }
  pthread_mutex_unlock(&global_mutex);
  YALOG_REF_RELEASE(old_config);
}
