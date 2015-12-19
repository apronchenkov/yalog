#include "yalog/core.h"
#include "yalog/logging.h"
#include "spinlock.h"
#include <limits.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

static pthread_mutex_t global_mutex = PTHREAD_MUTEX_INITIALIZER;
static Logger *global_loggers_head = NULL;
static const YalogConfig *global_config = NULL;

struct Logger {
  volatile int threshold;
  YalogSpinlock lock;
  YalogSink *sink;
  Logger *next;
  char tag[1];
};

Logger *GetLogger(const char *tag) {
  Logger *logger;
  pthread_mutex_lock(&global_mutex);
  for (logger = global_loggers_head; logger; logger = logger->next) {
    if (0 == strcmp(logger->tag, tag)) {
      pthread_mutex_unlock(&global_mutex);
      return logger;
    }
  }
  logger = malloc(sizeof(Logger) + strlen(tag));
  strcpy(logger->tag, tag);
  YalogSpinInit(&logger->lock);
  if (global_config) {
    logger->sink = global_config->GetSink(global_config, tag);
  } else {
    logger->sink = NULL;
  }
  if (logger->sink) {
    logger->threshold = logger->sink->threshold;
  } else {
    logger->threshold = INT_MAX;
  }
  logger->next = global_loggers_head;
  global_loggers_head = logger;
  pthread_mutex_unlock(&global_mutex);
  return logger;
}

void YalogSend(Logger *logger, int severity, const char *file, int line,
               const char *format, ...) {
  YalogSink *sink = NULL;
  YalogSpinLock(&logger->lock);
  if (logger->sink && logger->sink->threshold <= severity) {
    sink = YALOG_REF_ACQUIRE(logger->sink);
  }
  YalogSpinUnlock(&logger->lock);
  if (sink) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    YalogMessage message;
    message.severity = severity;
    message.tag = logger->tag;
    message.file = file;
    message.line = line;
    message.time_sec = tv.tv_sec;
    message.time_usec = tv.tv_usec;
    char text[128];
    va_list args;
    va_start(args, format);
    const int n = vsnprintf(text, sizeof(text), format, args);
    va_end(args);
    if (n < 0 || ((size_t)n) < sizeof(text)) {
      message.text = text;
      message.text_size = n;
      sink->Send(sink, &message);
    } else {
      char *big_text = malloc(n + 1);
      va_start(args, format);
      vsnprintf(big_text, n + 1, format, args);
      va_end(args);
      message.text = big_text;
      message.text_size = n;
      sink->Send(sink, &message);
      free(big_text);
    }
    YALOG_REF_RELEASE(sink);
  }
}

static void YalogSetSink(Logger *logger, YalogSink *sink) {
  YalogSpinLock(&logger->lock);
  YalogSink *old_sink = logger->sink;
  if (sink) {
    logger->sink = sink;
    logger->threshold = sink->threshold;
  } else {
    logger->sink = NULL;
    logger->threshold = INT_MAX;
  }
  YalogSpinUnlock(&logger->lock);
  YALOG_REF_RELEASE(old_sink);
}

void YalogSetConfig(const YalogConfig *config) {
  pthread_mutex_lock(&global_mutex);
  const YalogConfig *old_config = global_config;
  if (config) {
    for (Logger *logger = global_loggers_head; logger; logger = logger->next) {
      YalogSetSink(logger, config->GetSink(config, logger->tag));
    }
    global_config = config;
  } else {
    for (Logger *logger = global_loggers_head; logger; logger = logger->next) {
      YalogSetSink(logger, NULL);
    }
    global_config = NULL;
  }
  pthread_mutex_unlock(&global_mutex);
  YALOG_REF_RELEASE(old_config);
}
