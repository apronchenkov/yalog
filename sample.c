#include <github.com/apronchenkov/yalog/public/basic.h>
#include <github.com/apronchenkov/yalog/public/logging_printf.h>

int main() {
  YalogLogger *const logger = YalogGetLogger("123");
  YalogSetConfig(YalogCreatePlainConfig(YalogCreateStderrSink(YALOG_INFO)));
  YALOG_PRINTF(logger, DEBUG, "%s", "Hello, World!");
  YALOG_PRINTF(logger, INFO, "%s", "Hello, World!");
  YALOG_PRINTF(logger, WARNING, "%s", "Hello, World!");
  YALOG_PRINTF(logger, ERROR, "%s", "Hello, World!");
  YALOG_PRINTF(
      logger, CRITICAL, "%s",
      "Long long long long long long long long long long long long long long "
      "long long long long long long long long long long long long long long "
      "long long long long long long long long long long long long long long "
      "long long long long long long long long long long long long long long "
      "long long long long long long long long long long long long message.");

  YALOG_PRINTF(logger, INFO, "%.*s", 5, "123456789");
  YALOG_PRINTF(logger, INFO, "%d %d %d", 1, 2, 3);

  YalogSetConfig(NULL);
  YALOG_PRINTF(logger, DEBUG, "%s", "Hello, World!");
  YALOG_PRINTF(logger, INFO, "%s", "Hello, World!");
  YALOG_PRINTF(logger, WARNING, "%s", "Hello, World!");
  return 0;
}
