#include "public/backend.h"

YalogSink *YalogCreateStderrSink(int threshold) {
  return YalogCreateFileSink(threshold, stderr, false);
}
