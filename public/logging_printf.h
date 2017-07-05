#pragma once
#include "core.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

void YalogVPrintf(YalogLogger *logger, int severity, const char *file,
                  int file_line, const char *function, const char *format,
                  va_list args);

void YalogPrintf(YalogLogger *logger, int severity, const char *file,
                 int file_line, const char *function, const char *format, ...);

#define YALOG_PRINTF(logger, severity, ...)                       \
  if (!YalogIsLoggerEnabled((logger), (YALOG_##severity)))        \
    (void)0;                                                      \
  else                                                            \
    YalogPrintf((logger), (YALOG_##severity), __FILE__, __LINE__, \
                __PRETTY_FUNCTION__, __VA_ARGS__)

#ifdef __cplusplus
}  // extern "C"
#endif
