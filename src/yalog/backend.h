#ifndef __yalog_backend_h__
#define __yalog_backend_h__

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif

const YalogConfig *YalogCreatePlainConfig(YalogSink *sink);
YalogSink *YalogCreateStderrSink(int threshold);

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  //_yalog_backend_h__
