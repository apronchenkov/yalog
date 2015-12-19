#ifndef __yalog_syslog_send_h__
#define __yalog_syslog_send_h__ 1

#include <sys/uio.h>

#ifdef __cplusplus
extern "C" {
#endif

void YalogSyslogSend(const char *t1, size_t t1l, const char *t2, size_t t2l,
                     const char *t3, size_t t3l, const char *t4, size_t t4l);

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  //_yalog_syslog_send_h__
