#ifndef __yalog_core_h__
#define __yalog_core_h__ 1

#include "logging.h"
#include <stdatomic.h>
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
  atomic_uint ref_counter;
  void (*Destroy)(const YalogSink * /*self*/);

  int threshold;
  void (*Send)(YalogSink * /*self*/, const YalogMessage * /*message*/);
  void (*Flush)(YalogSink * /*self*/);
};

typedef struct YalogConfig YalogConfig;

struct YalogConfig {
  atomic_uint ref_counter;
  void (*Destroy)(const YalogConfig * /*self*/);

  YalogSink *(*GetSink)(const YalogConfig * /*self*/, const char * /*tag*/);
};

#define YALOG_REF_INIT(ptr, destroy) \
  ((ptr)->ref_counter = 1, (ptr)->Destroy = destroy, (ptr))

#define YALOG_REF_ACQUIRE(ptr)                                      \
  (atomic_fetch_add_explicit((atomic_uint *)&(ptr)->ref_counter, 1, \
                             memory_order_relaxed),                 \
   (ptr))

#define YALOG_REF_RELEASE(ptr)                                                \
  if (!(ptr) || atomic_fetch_sub_explicit((atomic_uint *)&(ptr)->ref_counter, \
                                          1, memory_order_relaxed))           \
    (void)0;                                                                  \
  else                                                                        \
  (ptr)->Destroy(ptr)

void YalogSetConfig(const YalogConfig *config);

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  //_yalog_core_h__
