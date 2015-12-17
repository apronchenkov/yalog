#include <yalog/core.h>
#include <yalog/logging.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

char SeverityChr(int severity) {
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

struct StderrSink {
  YalogSink base;
};

void StderrSink_Send(const YalogSink *self, const YalogMessage *message) {
  (void)self;
  struct tm tm;
  localtime_r(&message->time_sec, &tm);
  fprintf(stderr, "%c%02d%02d %02d:%02d:%02d.%06ld %ld %s:%d] %s] %s\n",
          SeverityChr(message->severity), tm.tm_mon + 1, tm.tm_mday, tm.tm_hour,
          tm.tm_min, tm.tm_sec, message->time_usec, (long)getpid(),
          message->file, message->line, message->tag, message->text);
}

void StderrSink_Destroy(const YalogSink *self) { free((void *)self); }
YalogSink *CreateStderrSink(int threshold) {
  struct StderrSink *self = malloc(sizeof(struct StderrSink));
  self->base.threshold = threshold;
  self->base.Send = StderrSink_Send;
  return YALOG_REF_INIT(&self->base, StderrSink_Destroy);
}

struct DefaultConfig {
  YalogConfig base;
  YalogSink *default_sink;
};

const YalogSink *DefaultConfig_GetSink(const YalogConfig *self,
                                       const char *tag) {
  (void)tag;
  return YALOG_REF_ACQUIRE(((const struct DefaultConfig *)self)->default_sink);
}

void DefaultConfig_Destroy(const YalogConfig *self) {
  YALOG_REF_RELEASE(((const struct DefaultConfig *)self)->default_sink);
  free((void *)self);
}

YalogConfig *CreateDefaultConfig(YalogSink *sink) {
  struct DefaultConfig *self = malloc(sizeof(struct DefaultConfig));
  self->default_sink = sink;
  self->base.GetSink = DefaultConfig_GetSink;
  return YALOG_REF_INIT(&self->base, DefaultConfig_Destroy);
}

int main() {
  Logger *const logger = GetLogger("123");
  YalogSetConfig(CreateDefaultConfig(CreateStderrSink(YALOG_INFO)));
  YALOG(logger, DEBUG, "%s", "Hello, World!");
  YALOG(logger, INFO, "%s", "Hello, World!");
  YALOG(logger, WARNING, "%s", "Hello, World!");
  YALOG(logger, CRITICAL, "%s",
        "Long long long long long long long long long long long long long long "
        "long long long long long long long long long long long long long long "
        "long long long long long long long long long long long long long long "
        "long long long long long long long long long long long long long long "
        "long long long long long long long long long long long long message.");

  YalogSetConfig(NULL);
  YALOG(logger, DEBUG, "%s", "Hello, World!");
  YALOG(logger, INFO, "%s", "Hello, World!");
  YALOG(logger, WARNING, "%s", "Hello, World!");
  return 0;
}
