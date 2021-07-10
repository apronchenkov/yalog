#pragma once
#include "core.h"

#include <stdbool.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

const YalogConfig *YalogCreatePlainConfig(YalogSink *sink);
YalogSink *YalogCreateFileSink(int threshold, FILE *file,
                               bool close_on_destroy);
YalogSink *YalogCreateStderrSink(int threshold);
YalogSink *YalogCreateSyslogSink(int threshold, const char *ident);

#ifdef __cplusplus
}  // extern "C"
#endif
