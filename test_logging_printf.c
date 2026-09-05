#include "@/public/basic.h"
#include "@/public/logging_printf.h"

#include <github.com/apronchenkov/u7_init/public/testing.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

// YalogSink::Send() only guarantees the message's `text` is valid for the
// duration of the call -- for the generic-format fast path it may point at
// YalogVPrintf's own stack buffer or a heap buffer it frees right after
// sending. So this fake must copy the bytes out, not retain message->text.
typedef struct FakeSink {
  YalogSink base;
  char text[256];
  int text_size;
} FakeSink;

static void FakeSink_Send(YalogSink* base, const YalogMessage* message) {
  FakeSink* self = (FakeSink*)base;
  self->text_size = message->text_size;
  size_t n = 0;
  if (message->text != NULL && message->text_size > 0) {
    n = (size_t)message->text_size < sizeof(self->text) - 1
            ? (size_t)message->text_size
            : sizeof(self->text) - 1;
    memcpy(self->text, message->text, n);
  }
  self->text[n] = '\0';
}

static void FakeSink_Destroy(YalogSink const* base) {
  (void)base;  // Stack-allocated in tests; nothing to free.
}

static void FakeSinkInit(FakeSink* self, int threshold) {
  self->base.threshold = threshold;
  self->base.Send = FakeSink_Send;
  self->text_size = -12345;  // Sentinel: overwritten only if a message lands.
  self->text[0] = '\0';
  YALOG_REF_INIT(&self->base, FakeSink_Destroy);
}

// Installs `sink` as the only sink, calls YalogVPrintf with `format`/`...`
// on yalog_default_logger, then restores a clean global config so the next
// test doesn't see a stack-allocated sink that has gone out of scope.
static void SendMessage(FakeSink* sink, int threshold, int severity,
                        const char* format, ...) {
  FakeSinkInit(sink, threshold);
  YalogSetConfig(YalogCreatePlainConfig(&sink->base));
  va_list args;
  va_start(args, format);
  YalogVPrintf(severity, __FILE__, __LINE__, __func__, yalog_default_logger,
               format, args);
  va_end(args);
  YalogSetConfig(NULL);
}

U7_TEST(test_percent_s_sends_full_string) {
  FakeSink sink;
  SendMessage(&sink, YALOG_DEBUG, YALOG_INFO, "%s", "hello");
  U7_ASSERT_EQ(sink.text_size, 5);
  U7_ASSERT_EQ(sink.text, "hello");
}

U7_TEST(test_percent_dot_star_s_respects_explicit_precision) {
  FakeSink sink;
  SendMessage(&sink, YALOG_DEBUG, YALOG_INFO, "%.*s", 3, "hello");
  U7_ASSERT_EQ(sink.text_size, 3);
  U7_ASSERT_EQ(sink.text, "hel");
}

U7_TEST(test_percent_dot_star_s_negative_precision_sends_full_string) {
  FakeSink sink;
  // A negative precision means "no precision" per printf's own convention.
  // Before YalogSafeStrLen, this flowed straight into message.text_size,
  // which the syslog sink casts to size_t for an iovec length.
  SendMessage(&sink, YALOG_DEBUG, YALOG_INFO, "%.*s", -1, "hello");
  U7_ASSERT_EQ(sink.text_size, 5);
  U7_ASSERT_EQ(sink.text, "hello");
}

U7_TEST(test_percent_dot_star_s_clamps_precision_past_nul) {
  FakeSink sink;
  SendMessage(&sink, YALOG_DEBUG, YALOG_INFO, "%.*s", 100, "hi");
  U7_ASSERT_EQ(sink.text_size, 2);
  U7_ASSERT_EQ(sink.text, "hi");
}

U7_TEST(test_empty_format_sends_no_text) {
  FakeSink sink;
  SendMessage(&sink, YALOG_DEBUG, YALOG_INFO, "");
  U7_ASSERT_EQ(sink.text_size, 0);
}

U7_TEST(test_generic_format_short_message) {
  FakeSink sink;
  SendMessage(&sink, YALOG_DEBUG, YALOG_INFO, "n=%d", 42);
  U7_ASSERT_EQ(sink.text_size, 4);
  U7_ASSERT_EQ(sink.text, "n=42");
}

U7_TEST(test_generic_format_long_message_uses_heap_buffer) {
  FakeSink sink;
  char long_arg[200];
  memset(long_arg, 'x', sizeof(long_arg) - 1);
  long_arg[sizeof(long_arg) - 1] = '\0';
  char expected[210];
  const int expected_len =
      snprintf(expected, sizeof(expected), "[%s]", long_arg);
  SendMessage(&sink, YALOG_DEBUG, YALOG_INFO, "[%s]", long_arg);
  U7_ASSERT_EQ(sink.text_size, expected_len);
  U7_ASSERT_EQ(sink.text, expected);
}

U7_TEST(test_below_threshold_is_not_sent) {
  FakeSink sink;
  FakeSinkInit(&sink, YALOG_WARNING);
  YalogSetConfig(YalogCreatePlainConfig(&sink.base));
  YALOG_PRINTF(INFO, "should not be delivered");
  U7_ASSERT_EQ(sink.text_size, -12345);  // Sentinel: untouched.
  YalogSetConfig(NULL);
}

int main(int argc, char** argv) {
  return u7_testing_run_registered(argc, argv);
}
