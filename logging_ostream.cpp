#include "public/logging_ostream.h"
#include <memory>
#include <sys/time.h>

namespace yalog {
namespace {

YalogMessage initYalogMessage(int severity, YalogLogger *logger,
                              const char *file, int file_line,
                              const char *function) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  YalogMessage result;
  result.severity = severity;
  result.category = YalogLoggerGetCategory(logger);
  result.file = file;
  result.file_line = file_line;
  result.function = function;
  result.unix_time = tv.tv_sec + 1e-6 * tv.tv_usec;
  result.text = nullptr;
  result.text_size = 0;
  return result;
}

class StreamBuf : public std::streambuf {
 public:
  StreamBuf(char *buffer, int size) { setp(buffer, buffer + size); }

  int count() const { return pptr() - pbase(); }
};

constexpr const int MESSAGE_MAX_SIZE = 8192;

}  // namespace

class Message::Impl {
 public:
  Impl(int severity, YalogLogger *logger, const char *file, int file_line,
       const char *function)
      : message_(initYalogMessage(severity, logger, file, file_line, function)),
        logger_(logger),
        buffer_(new char[MESSAGE_MAX_SIZE]),
        streambuf_(buffer_.get(), MESSAGE_MAX_SIZE),
        ostream_(&streambuf_) {}

  ~Impl() {
    message_.text = buffer_.get();
    message_.text_size = streambuf_.count();
    YalogLoggerSend(logger_, &message_);
  }

  std::ostream &GetOStream() { return ostream_; }

 private:
  YalogMessage message_;
  YalogLogger *logger_;
  std::unique_ptr<char[]> buffer_;
  StreamBuf streambuf_;
  std::ostream ostream_;
};

Message::Message(int severity, YalogLogger *logger, const char *file,
                 int file_line, const char *function)
    : impl_(new Impl(severity, logger, file, file_line, function)) {}

Message::Message(int severity, const char *file, int file_line,
                 const char *function)
    : impl_(new Impl(severity, ::default_logger, file, file_line, function)) {}

Message::~Message() noexcept { delete impl_; }

std::ostream &Message::GetOStream() { return impl_->GetOStream(); }

}  // namespace yalog
