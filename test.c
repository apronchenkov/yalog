#include <yalog/backend.h>
#include <yalog/logging.h>

int main() {
  Logger *const logger = GetLogger("123");
  YalogSetConfig(YalogCreatePlainConfig(YalogCreateSyslogSink(YALOG_INFO, "main_test")));
  YALOG(logger, DEBUG, "%s", "Hello, World!");
  YALOG(logger, INFO, "%s", "Hello, World!");
  YALOG(logger, WARNING, "%s", "Hello, World!");
  YALOG(logger, ERROR, "%s", "Hello, World!");
  YALOG(logger, CRITICAL, "%s",
        "Long long long long long long long long long long long long long long "
        "long long long long long long long long long long long long long long "
        "long long long long long long long long long long long long long long "
        "long long long long long long long long long long long long long long "
        "long long long long long long long long long long long long message.");

  YalogSetConfig(NULL);
  YALOG(logger, DEBUG, "%s", "Hello, World!");
  YALOG(logger, INFO, "%s", "Hello, World!");
  YALOG(logger, WARNING, "%s", "Hello, World!");
  return 0;
}
