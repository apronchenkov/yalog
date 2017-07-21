#include <github.com/apronchenkov/yalog/public/basic.h>
#include <github.com/apronchenkov/yalog/public/logging_ostream.h>

int main() {
  YalogLogger *const logger = YalogGetLogger("123");
  YalogSetConfig(YalogCreatePlainConfig(YalogCreateStderrSink(YALOG_INFO)));
  YALOG_OSTREAM(DEBUG, logger) << "Hello, World!";
  YALOG_OSTREAM(INFO, logger) << "Hello, World!";
  YALOG_OSTREAM(WARNING, logger) << "Hello, World!";
  YALOG_OSTREAM(ERROR, logger) << "Hello, World!";
  YALOG_OSTREAM(CRITICAL, logger)
      << "Long long long long long long long long long long long long long "
         "long long long long long long long long long long long long long "
         "long long long long long long long long long long long long long "
         "long long long long long long long long long long long long long "
         "long long long long long long long long long long long long long "
         "long long long message.";

  YALOG_OSTREAM(INFO, logger) << "12345";
  YALOG_OSTREAM(INFO, logger) << 1 << ' ' << 2 << ' ' << 3;

  YalogSetConfig(NULL);
  YALOG_OSTREAM(DEBUG, logger) << "Hello, World!";
  YALOG_OSTREAM(INFO, logger) << "Hello, World!";
  YALOG_OSTREAM(WARNING, logger) << "Hello, World!";
  return 0;
}
