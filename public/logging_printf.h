#pragma once
#include "core.h"

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

void YalogVPrintf(int severity, const char *file, int file_line,
                  const char *function, YalogLogger *logger, const char *format,
                  va_list args);

void YalogPrintf_1(int severity, const char *file, int file_line,
                   const char *function, YalogLogger *logger,
                   const char *format, ...)
    __attribute__((format(printf, 6, 7)));

void YalogPrintf_2(int severity, const char *file, int file_line,
                   const char *function, const char *format, ...)
    __attribute__((format(printf, 5, 6)));

#ifdef __cplusplus
}  // extern "C"
#endif

#ifndef __cplusplus

#define YALOG_PRINTF_HEAD(head, ...) (head)

#define YALOG_PRINTF_GET_LOGGER(arg) \
  _Generic((arg), YalogLogger * : (arg), default : (yalog_default_logger))

#define YALOG_PRINTF_GET_IMPL(arg) \
  _Generic((arg), YalogLogger * : YalogPrintf_1, default : YalogPrintf_2)

#define YALOG_PRINTF(severity, ...)                                           \
  if (!YalogIsLoggerEnabled(                                                  \
          YALOG_PRINTF_GET_LOGGER(YALOG_PRINTF_HEAD(__VA_ARGS__, NULL)),      \
          (YALOG_##severity)))                                                \
    (void)0;                                                                  \
  else                                                                        \
    YALOG_PRINTF_GET_IMPL(YALOG_PRINTF_HEAD(__VA_ARGS__, NULL))               \
  ((YALOG_##severity), __FILE__, __LINE__, __extension__ __PRETTY_FUNCTION__, \
   __VA_ARGS__)

#else

#include <type_traits>

template <typename T>
static inline YalogLogger *YalogPrintf_GetLogger(T *arg) {
  if constexpr (std::is_base_of_v<YalogLogger, T>) {
    return arg;
  } else {
    (void)arg;
    return yalog_default_logger;
  }
}

template <typename T>
static inline auto YalogPrintf_GetImpl(T * /*arg*/) {
  if constexpr (std::is_base_of_v<YalogLogger, T>) {
    return &YalogPrintf_1;
  } else {
    return &YalogPrintf_2;
  }
}

#define YALOG_PRINTF_HEAD(head, ...) (head)

#define YALOG_PRINTF(severity, ...)                                    \
  if (!YalogIsLoggerEnabled(                                           \
          YalogPrintf_GetLogger(YALOG_PRINTF_HEAD(__VA_ARGS__, NULL)), \
          (YALOG_##severity)))                                         \
    (void)0;                                                           \
  else                                                                 \
    YalogPrintf_GetImpl(YALOG_PRINTF_HEAD(__VA_ARGS__, NULL))(         \
        (YALOG_##severity), __FILE__, __LINE__,                        \
        __extension__ __PRETTY_FUNCTION__, __VA_ARGS__)

#endif
