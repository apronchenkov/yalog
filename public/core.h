#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define YALOG_DEBUG (0)
#define YALOG_INFO (1)
#define YALOG_WARNING (2)
#define YALOG_ERROR (3)
#define YALOG_CRITICAL (4)

typedef struct YalogMessage YalogMessage;

struct YalogMessage {
  int severity;
  const char *category;
  const char *file;
  int file_line;
  const char *function;
  const char *text;
  int text_size;
  double unix_time;
};

typedef struct YalogLogger YalogLogger;

extern YalogLogger *const default_logger;

YalogLogger *YalogGetLogger(const char *category);

const char *YalogLoggerGetCategory(YalogLogger *logger);

void YalogLoggerSend(YalogLogger *logger, const YalogMessage *message);

static inline int YalogIsLoggerEnabled(YalogLogger *logger, int severity) {
  return __atomic_load_n((int *)logger, __ATOMIC_RELAXED) <= severity;
}

typedef struct YalogSink YalogSink;

struct YalogSink {
  unsigned int ref_counter;
  void (*Destroy)(const YalogSink * /*self*/);

  int threshold;
  void (*Send)(YalogSink * /*self*/, const YalogMessage * /*message*/);
};

typedef struct YalogConfig YalogConfig;

struct YalogConfig {
  volatile unsigned int ref_counter;
  void (*Destroy)(const YalogConfig * /*self*/);

  YalogSink *(*GetSink)(const YalogConfig * /*self*/,
                        const char * /*category*/);
};

void YalogSetConfig(const YalogConfig *config);

#define YALOG_REF_INIT(ptr, destroy) \
  ((ptr)->ref_counter = 1, (ptr)->Destroy = destroy, (ptr))

#define YALOG_REF_ACQUIRE(ptr)                                \
  (__atomic_fetch_add((unsigned int *)&(ptr)->ref_counter, 1, \
                      __ATOMIC_RELAXED),                      \
   (ptr))

#define YALOG_REF_RELEASE(ptr)                                             \
  if (!(ptr) || __atomic_sub_fetch((unsigned int *)&(ptr)->ref_counter, 1, \
                                   __ATOMIC_RELAXED))                      \
    (void)0;                                                               \
  else                                                                     \
    (ptr)->Destroy(ptr)

#ifdef __cplusplus
}  // extern "C"
#endif
