#pragma once

#ifndef __cplusplus
#error "This interface is available for C++ only."
#endif

#include "core.h"
#include <ostream>

namespace yalog {

class Message {
 public:
  Message(YalogLogger *logger, int severity, const char *file, int file_line,
          const char *function);

  ~Message() noexcept;

  template <typename T>
  Message &operator<<(const T &item);

  /* Movable but non-copyable */
  Message() noexcept : impl_(nullptr) {}

  Message(Message &&message) noexcept : impl_(message.impl_) {
    message.impl_ = nullptr;
  }

  Message &operator=(Message &&message) noexcept {
    impl_ = message.impl_;
    message.impl_ = nullptr;
    return *this;
  }

  Message(const Message &) noexcept = delete;

  Message &operator=(const Message &message) noexcept = delete;

 private:
  std::ostream &GetOStream();

  class Impl;
  Impl *impl_;
};

template <typename T>
Message &Message::operator<<(const T &item) {
  if (impl_) {
    GetOStream() << item;
  }
  return *this;
}

}  // namespace yalog

#define YALOG_OSTREAM(logger, severity)                                \
  if (!YalogIsLoggerEnabled((logger), (YALOG_##severity)))             \
    (void)0;                                                           \
  else                                                                 \
    ::yalog::Message((logger), (YALOG_##severity), __FILE__, __LINE__, \
                     __PRETTY_FUNCTION__)
