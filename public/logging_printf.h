#pragma once
#include "core.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

void YalogVPrintf(int severity, YalogLogger *logger, const char *file,
                  int file_line, const char *function, const char *format,
                  va_list args);

void YalogPrintf(int severity, YalogLogger *logger, const char *file,
                 int file_line, const char *function, const char *format, ...)
    __attribute__((format(printf, 6, 7)));

#define YALOG_PRINTF_GET_LOGGER(arg) \
  _Generic((arg), YalogLogger * : (arg), default : default_logger)

#define YALOG_PRINTF(severity, arg, ...)                                       \
  if (!YalogIsLoggerEnabled(YALOG_PRINTF_GET_LOGGER(arg), (YALOG_##severity))) \
    (void)0;                                                                   \
  else                                                                         \
    YalogPrintf((YALOG_##severity), YALOG_PRINTF_GET_LOGGER(arg), __FILE__,    \
                __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__)

#ifdef __cplusplus
}  // extern "C"
#endif
