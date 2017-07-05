#include "public/basic.h"
#include <common/logging/syslog_client/syslog_client.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct YalogSyslogSink YalogSyslogSink;

struct YalogSyslogSink {
  YalogSink base;
  SyslogClient *syslog_client;
};

static inline int GetSyslogSeverity(int severity) {
  if (severity <= YALOG_DEBUG) {
    return SYSLOG_SEVERITY_DEBUG;
  } else if (severity <= YALOG_INFO) {
    return SYSLOG_SEVERITY_INFO;
  } else if (severity <= YALOG_WARNING) {
    return SYSLOG_SEVERITY_WARNING;
  } else if (severity <= YALOG_ERROR) {
    return SYSLOG_SEVERITY_ERROR;
  } else {
    return SYSLOG_SEVERITY_CRITICAL;
  }
}

static void YalogSyslogSink_Send(YalogSink *self, const YalogMessage *message) {
  struct timeval tv;
  tv.tv_sec = (time_t)floor(message->unix_time);
  tv.tv_usec = lround(floor(1e6 * fmod(message->unix_time, 1.0)));
  SyslogClientSend(((YalogSyslogSink *)self)->syslog_client,
                   GetSyslogSeverity(message->severity), &tv, message->text,
                   message->text_size);
  // We have nothing to do if it fails.
}

static void YalogSyslogSink_Destroy(const YalogSink *self) {
  SyslogClientDestroy(((YalogSyslogSink *)self)->syslog_client);
  free((void *)self);
}

YalogSink *YalogCreateSyslogSink(int threshold, const char *ident) {
  YalogSyslogSink *self = malloc(sizeof(YalogSyslogSink));
  self->base.threshold = threshold;
  self->base.Send = YalogSyslogSink_Send;
  self->syslog_client = SyslogClientCreateDefault(SYSLOG_FACILITY_USER, ident);
  if (!self->syslog_client) {
    const int errno_copy = errno;
    free(self);
    errno = errno_copy;
    return NULL;
  }
  return YALOG_REF_INIT(&self->base, YalogSyslogSink_Destroy);
}
