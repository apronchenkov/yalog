#ifndef __yalog_logging_h__
#define __yalog_logging_h__ 1

#ifdef __cplusplus
extern "C" {
#endif

#define YALOG_DEBUG 0
#define YALOG_INFO 1
#define YALOG_WARNING 2
#define YALOG_ERROR 3
#define YALOG_CRITICAL 4

typedef struct Logger Logger;

Logger *GetLogger(const char *tag);

static inline int YalogGetLoggerThreshold(Logger *logger) {
  return __atomic_load_n((int *)logger, __ATOMIC_RELAXED);
}

void YalogSend(Logger *logger, int severity, const char *file, int line,
               const char *format, ...);

#define YALOG(logger, severity, format, ...)                \
  if (YalogGetLoggerThreshold(logger) > (YALOG_##severity)) \
    (void)0;                                                \
  else                                                      \
  YalogSend(logger, (YALOG_##severity), __FILE__, __LINE__, format, __VA_ARGS__)

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  //_yalog_logging_h__
