#ifndef __yalog_backend_h__
#define __yalog_backend_h__

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
#endif  //_yalog_backend_h__
