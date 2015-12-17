#include "yalog/backend.h"

YalogSink *YalogCreateStderrSink(int threshold) {
  return YalogCreateFileSink(threshold, stderr, false);
}
