#ifndef __yalog_core_h__
#define __yalog_core_h__ 1

#include "logging.h"
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct YalogMessage YalogMessage;

struct YalogMessage {
  int severity;
  const char *tag;
  const char *file;
  int line;
  int text_size;
  const char *text;
  time_t time_sec;
  long time_usec;
};

typedef struct YalogSink YalogSink;

struct YalogSink {
  unsigned int ref_counter;
  void (*Destroy)(const YalogSink * /*self*/);

  int threshold;
  void (*Send)(YalogSink * /*self*/, const YalogMessage * /*message*/);
  void (*Flush)(YalogSink * /*self*/);
};

typedef struct YalogConfig YalogConfig;

struct YalogConfig {
  unsigned int ref_counter;
  void (*Destroy)(const YalogConfig * /*self*/);

  YalogSink *(*GetSink)(const YalogConfig * /*self*/, const char * /*tag*/);
};

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

void YalogSetConfig(const YalogConfig *config);

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  //_yalog_core_h__
