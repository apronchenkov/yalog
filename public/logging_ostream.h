#pragma once

#ifndef __cplusplus
#error "This interface is available for C++ only."
#endif

#include "core.h"

#include <ostream>

namespace yalog {

class Message {
 public:
  Message(int severity, YalogLogger* logger, const char* file, int file_line,
          const char* function);

  Message(int severity, const char* file, int file_line, const char* function);

  ~Message() noexcept;

  template <typename T>
  Message& operator<<(const T& item);

  /* Movable but non-copyable */
  Message() noexcept : impl_(nullptr) {}

  Message(Message&& message) noexcept : impl_(message.impl_) {
    message.impl_ = nullptr;
  }

  Message& operator=(Message&& message) noexcept {
    impl_ = message.impl_;
    message.impl_ = nullptr;
    return *this;
  }

  Message(const Message&) noexcept = delete;

  Message& operator=(const Message& message) noexcept = delete;

 private:
  std::ostream& GetOStream();

  class Impl;
  Impl* impl_;
};

template <typename T>
Message& Message::operator<<(const T& item) {
  if (impl_) {
    GetOStream() << item;
  }
  return *this;
}

inline bool IsLoggerEnabled(int severity, YalogLogger* logger) {
  return YalogIsLoggerEnabled(logger, severity);
}

inline bool IsLoggerEnabled(int severity) {
  return YalogIsLoggerEnabled(::yalog_default_logger, severity);
}

}  // namespace yalog

#define YALOG_OSTREAM(/*severity, logger*/...)                \
  if (!::yalog::IsLoggerEnabled(YALOG_##__VA_ARGS__))         \
    (void)0;                                                  \
  else                                                        \
    ::yalog::Message(YALOG_##__VA_ARGS__, __FILE__, __LINE__, \
                     __PRETTY_FUNCTION__)
