#pragma once

#include <github.com/apronchenkov/u7_init/public/refcount.h>

#ifdef __cplusplus
extern "C" {
#endif

#define YALOG_DEBUG (0)
#define YALOG_INFO (1)
#define YALOG_WARNING (2)
#define YALOG_ERROR (3)
#define YALOG_CRITICAL (4)

// An opaque structure representing a logger.
//
// Ownership: Each logger object is a singletone. It's safe to pass a pointer
// between threads.
typedef struct YalogLogger YalogLogger;

// A message for logging.
//
// This structure represents an interface, and doesn't hold ownership.
// If you need to pass a message between threads, please create a deep copy.
struct YalogMessage {
  int severity;
  const char* category;
  const char* file;
  int file_line;
  const char* function;
  const char* text;
  int text_size;
  double unix_time;
};

typedef struct YalogMessage YalogMessage;

// Default logger corresponding to an empty category name.
extern YalogLogger* const yalog_default_logger;

// Returns the logger instance by the category name.
YalogLogger* YalogGetLogger(const char* category);

// Returns the category name corresponding to the logger.
const char* YalogLoggerGetCategory(YalogLogger* logger);

// Sends a message to the logger.
void YalogLoggerSend(YalogLogger* logger, const YalogMessage* message);

// Returns non-zero if the logger is enabled for a particular serverity level.
static inline int YalogIsLoggerEnabled(YalogLogger* logger, int severity) {
  return __atomic_load_n((int*)logger, __ATOMIC_RELAXED) <= severity;
}

// An interface for a logging message consumer.
//
// Ownership is represented by ref-counting. Please use YALOG_REF_ACQUIRE and
// YALOG_REF_RELEASE macros to manipulate it.
struct YalogSink {
  u7_refcount refcount;

  // Threshold associated with the instance.
  int threshold; /* immutable */

  // Releases resources associated with the instance.
  void (*Destroy)(struct YalogSink const* /*self*/);

  // Send a message to this sink.
  void (*Send)(struct YalogSink* /*self*/, const YalogMessage* /*message*/);
};

typedef struct YalogSink YalogSink;

// An interface for a logger configuration.
//
// Ownership is represented by ref-counting. Please use YALOG_REF_ACQUIRE and
// YALOG_REF_RELEASE macros to manipulate it.
struct YalogConfig {
  u7_refcount refcount;

  // Releases resources associated with the object.
  void (*Destroy)(struct YalogConfig const* /*self*/);

  // Returns a yalog sink instance corresponding to the specified category.
  YalogSink* (*GetSink)(struct YalogConfig const* /*self*/,
                        const char* /*category*/);
};

typedef struct YalogConfig YalogConfig;

// Activate a new logging configuration.
void YalogSetConfig(const YalogConfig* config);

#define YALOG_REF_INIT(ptr, destroy) \
  (u7_refcount_init(&(ptr)->refcount), (ptr)->Destroy = destroy, (ptr))

#define YALOG_REF_ACQUIRE(ptr) (u7_refcount_increment(&(ptr)->refcount), (ptr))

#define YALOG_REF_RELEASE(ptr)                           \
  if (!(ptr) || u7_refcount_decrement(&(ptr)->refcount)) \
    (void)0;                                             \
  else                                                   \
    (ptr)->Destroy(ptr)

#ifdef __cplusplus
}  // extern "C"
#endif
