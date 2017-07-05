#include "public/basic.h"
#include <stdlib.h>

typedef struct YalogPlainConfig YalogPlainConfig;

struct YalogPlainConfig {
  YalogConfig base;
  YalogSink *sink;
};

static YalogSink *YalogPlainConfig_GetSink(const YalogConfig *self,
                                           const char *category) {
  (void)category;
  return YALOG_REF_ACQUIRE(((const YalogPlainConfig *)self)->sink);
}

static void YalogPlainConfig_Destroy(const YalogConfig *self) {
  YALOG_REF_RELEASE(((const YalogPlainConfig *)self)->sink);
  free((void *)self);
}

const YalogConfig *YalogCreatePlainConfig(YalogSink *sink) {
  YalogPlainConfig *self = malloc(sizeof(YalogPlainConfig));
  self->sink = sink;
  self->base.GetSink = YalogPlainConfig_GetSink;
  return YALOG_REF_INIT(&self->base, YalogPlainConfig_Destroy);
}
