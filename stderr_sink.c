#include "public/basic.h"

YalogSink *YalogCreateStderrSink(int threshold) {
  return YalogCreateFileSink(threshold, stderr, false);
}
