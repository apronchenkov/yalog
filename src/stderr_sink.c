#include "yalog/backend.h"
#include "yalog/core.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static inline char SeverityChr(int severity) {
  switch (severity) {
    case YALOG_DEBUG:
      return 'D';
    case YALOG_INFO:
      return 'I';
    case YALOG_WARNING:
      return 'W';
    case YALOG_ERROR:
      return 'E';
    case YALOG_CRITICAL:
      return 'C';
    default:
      return '?';
  }
}

typedef struct YalogStderrSink YalogStderrSink;

struct YalogStderrSink {
  YalogSink base;
};

static void YalogStderrSink_Send(YalogSink *self, const YalogMessage *message) {
  (void)self;
  struct tm tm;
  localtime_r(&message->time_sec, &tm);
  fprintf(stderr, "%c%02d%02d %02d:%02d:%02d.%06ld %ld %s:%d] %s] %s\n",
          SeverityChr(message->severity), tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
          tm.tm_min, tm.tm_sec, message->time_usec, (long)getpid(),
          message->file, message->line, message->tag, message->text);
}

static void YalogStderrSink_Destroy(const YalogSink *self) {
  free((void *)self);
}

YalogSink *YalogCreateStderrSink(int threshold) {
  YalogStderrSink *self = malloc(sizeof(YalogStderrSink));
  self->base.threshold = threshold;
  self->base.Send = YalogStderrSink_Send;
  return YALOG_REF_INIT(&self->base, YalogStderrSink_Destroy);
}
