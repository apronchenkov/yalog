#include "@/public/basic.h"
#include "@/public/logging_printf.h"

int main() {
  YalogLogger* const logger = YalogGetLogger("123");
  YalogSetConfig(YalogCreatePlainConfig(YalogCreateStderrSink(YALOG_INFO)));

  YALOG_PRINTF(INFO, "Hello, World!");
  YALOG_PRINTF(INFO, "%s, %s!", "Hello", "World");
  YALOG_PRINTF(INFO, logger, "%s, %s!", "Hello", "World");

  YALOG_PRINTF(DEBUG, logger, "%s", "Hello, World!");
  YALOG_PRINTF(INFO, "%s", "Hello, World!");
  YALOG_PRINTF(WARNING, logger, "%s", "Hello, World!");
  YALOG_PRINTF(ERROR, logger, "%s", "Hello, World!");
  YALOG_PRINTF(
      CRITICAL, logger, "%s",
      "Long long long long long long long long long long long long long long "
      "long long long long long long long long long long long long long long "
      "long long long long long long long long long long long long long long "
      "long long long long long long long long long long long long long long "
      "long long long long long long long long long long long long message.");

  YALOG_PRINTF(INFO, logger, "%.*s", 5, "123456789");
  YALOG_PRINTF(INFO, logger, "%d %d %d", 1, 2, 3);

  YalogSetConfig(NULL);
  YALOG_PRINTF(DEBUG, logger, "%s", "Hello, World!");
  YALOG_PRINTF(INFO, logger, "%s", "Hello, World!");
  YALOG_PRINTF(WARNING, logger, "%s", "Hello, World!");
  return 0;
}
