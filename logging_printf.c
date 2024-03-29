#include "@/public/logging_printf.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

void YalogVPrintf(int severity, const char* file, int file_line,
                  const char* function, YalogLogger* logger, const char* format,
                  va_list args) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  YalogMessage message;
  message.severity = severity;
  message.category = YalogLoggerGetCategory(logger);
  message.file = file;
  message.file_line = file_line;
  message.function = function;
  message.unix_time = tv.tv_sec + 1e-6 * tv.tv_usec;
  if (format[0] == '\0') {
    message.text = NULL;
    message.text_size = 0;
    YalogLoggerSend(logger, &message);
  } else if (format[0] == '%' && format[1] == 's' && format[2] == '\0') {
    message.text = va_arg(args, const char*);
    message.text_size = strlen(message.text);
    YalogLoggerSend(logger, &message);
  } else if (format[0] == '%' && format[1] == '.' && format[2] == '*' &&
             format[3] == 's' && format[4] == '\0') {
    message.text_size = va_arg(args, int);
    message.text = va_arg(args, const char*);
    YalogLoggerSend(logger, &message);
  } else {
    char text[128];
    va_list args_copy;
    va_copy(args_copy, args);
    const int n = vsnprintf(text, sizeof(text), format, args_copy);
    va_end(args_copy);
    if (n < 0) {
      // Unable to generate the text. It's likely that formatting string is
      // invalid.
      //
      // FIXME: Should we abort in that case?
    } else if (((size_t)n) < sizeof(text)) {
      message.text = text;
      message.text_size = n;
      YalogLoggerSend(logger, &message);
    } else {
      char* big_text = malloc(n + 1);
      vsnprintf(big_text, n + 1, format, args);
      message.text = big_text;
      message.text_size = n;
      YalogLoggerSend(logger, &message);
      free(big_text);
    }
  }
}

void YalogPrintf_1(int severity, const char* file, int file_line,
                   const char* function, YalogLogger* logger,
                   const char* format, ...) {
  va_list args;
  va_start(args, format);
  YalogVPrintf(severity, file, file_line, function, logger, format, args);
  va_end(args);
}

void YalogPrintf_2(int severity, const char* file, int file_line,
                   const char* function, const char* format, ...) {
  va_list args;
  va_start(args, format);
  YalogVPrintf(severity, file, file_line, function, yalog_default_logger,
               format, args);
  va_end(args);
}
