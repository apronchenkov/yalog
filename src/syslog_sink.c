#include "yalog/backend.h"
#include "syslog_send.h"
#include "yalog/core.h"
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

typedef struct YalogSyslogSink YalogSyslogSink;

struct YalogSyslogSink {
  YalogSink base;
  int ident_size;
  char ident[1];
};

static inline int Pri(int severity) {
  if (severity <= YALOG_DEBUG) {
    return LOG_USER | LOG_DEBUG;
  } else if (severity <= YALOG_INFO) {
    return LOG_USER | LOG_INFO;
  } else if (severity <= YALOG_WARNING) {
    return LOG_USER | LOG_WARNING;
  } else if (severity <= YALOG_ERROR) {
    return LOG_USER | LOG_ERR;
  } else {
    return LOG_USER | LOG_CRIT;
  }
}

static void YalogSyslogSink_Send(YalogSink *self, const YalogMessage *message) {
  struct tm tm;
  localtime_r(&message->time_sec, &tm);
  char header[32];
  size_t header_size = 0;
  header_size += snprintf(header + header_size, sizeof(header) - header_size,
                          "<%d>", Pri(message->severity));
  header_size += strftime(header + header_size, sizeof(header) - header_size,
                          "%h %e %T ", &tm);
  char pid[32];
  size_t pid_size = snprintf(pid, sizeof(pid), "[%lu]: ", (long)getpid());
  YalogSyslogSend(header, header_size, ((YalogSyslogSink *)self)->ident,
                  ((YalogSyslogSink *)self)->ident_size, pid, pid_size,
                  message->text, message->text_size);
}

static void YalogSyslogSink_Flush(YalogSink *self) { (void)self; }

static void YalogSyslogSink_Destroy(const YalogSink *self) {
  free((void *)self);
}

YalogSink *YalogCreateSyslogSink(int threshold, const char *ident) {
  const size_t ident_size = strlen(ident);
  YalogSyslogSink *self = malloc(sizeof(YalogSyslogSink) + ident_size);
  self->base.threshold = threshold;
  self->base.Send = YalogSyslogSink_Send;
  self->base.Flush = YalogSyslogSink_Flush;
  self->ident_size = ident_size;
  strcpy(self->ident, ident);
  return YALOG_REF_INIT(&self->base, YalogSyslogSink_Destroy);
}
