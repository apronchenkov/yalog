#define _BSD_SOURCE
#include "public/basic.h"
#include <math.h>
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

typedef struct YalogFileSink YalogFileSink;

struct YalogFileSink {
  YalogSink base;
  FILE *file;
  bool close_on_destroy;
};

static void YalogFileSink_Send(YalogSink *self, const YalogMessage *message) {
  struct tm tm;
  time_t unix_time_sec = (time_t)floor(message->unix_time);
  localtime_r(&unix_time_sec, &tm);
  fprintf(((YalogFileSink *)self)->file,
          "%c%02d%02d %02d:%02d:%02d.%06ld %ld %s:%d] %s] %.*s\n",
          SeverityChr(message->severity), tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
          tm.tm_min, tm.tm_sec,
          (long)floor(1e6 * fmod(message->unix_time, 1.0)), (long)getpid(),
          message->file, message->file_line, message->category,
          message->text_size, message->text);
}

static void YalogFileSink_Destroy(const YalogSink *self) {
  if (((const YalogFileSink *)self)->close_on_destroy) {
    if (fclose(((const YalogFileSink *)self)->file)) {
      fclose(((const YalogFileSink *)self)->file);  // second try
    }
  }
  free((void *)self);
}

YalogSink *YalogCreateFileSink(int threshold, FILE *file,
                               bool close_on_destroy) {
  YalogFileSink *self = malloc(sizeof(YalogFileSink));
  self->base.threshold = threshold;
  self->base.Send = YalogFileSink_Send;
  self->file = file;
  self->close_on_destroy = close_on_destroy;
  return YALOG_REF_INIT(&self->base, YalogFileSink_Destroy);
}
