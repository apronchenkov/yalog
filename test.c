#include <yalog/backend.h>
#include <yalog/logging.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>


int main() {
  Logger *const logger = GetLogger("123");
  YalogSetConfig(YalogCreatePlainConfig(YalogCreateStderrSink(YALOG_INFO)));
  YALOG(logger, DEBUG, "%s", "Hello, World!");
  YALOG(logger, INFO, "%s", "Hello, World!");
  YALOG(logger, WARNING, "%s", "Hello, World!");
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
