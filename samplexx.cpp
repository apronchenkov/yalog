#include <github.com/apronchenkov/yalog/public/basic.h>
#include <github.com/apronchenkov/yalog/public/logging_ostream.h>

int main() {
  YalogLogger *const logger = YalogGetLogger("123");
  YalogSetConfig(YalogCreatePlainConfig(YalogCreateStderrSink(YALOG_INFO)));
  YALOG_OSTREAM(logger, DEBUG) << "Hello, World!";
  YALOG_OSTREAM(logger, INFO) << "Hello, World!";
  YALOG_OSTREAM(logger, WARNING) << "Hello, World!";
  YALOG_OSTREAM(logger, ERROR) << "Hello, World!";
  YALOG_OSTREAM(logger, CRITICAL)
      << "Long long long long long long long long long long long long long "
         "long long long long long long long long long long long long long "
         "long long long long long long long long long long long long long "
         "long long long long long long long long long long long long long "
         "long long long long long long long long long long long long long "
         "long long long message.";

  YALOG_OSTREAM(logger, INFO) << "12345";
  YALOG_OSTREAM(logger, INFO) << 1 << ' ' << 2 << ' ' << 3;

  YalogSetConfig(NULL);
  YALOG_OSTREAM(logger, DEBUG) << "Hello, World!";
  YALOG_OSTREAM(logger, INFO) << "Hello, World!";
  YALOG_OSTREAM(logger, WARNING) << "Hello, World!";
  return 0;
}
