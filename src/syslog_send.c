#include "syslog_send.h"
#include "unix_socket.h"
#include "spinlock.h"
#include <syslog.h>
#include <pthread.h>

static YalogSpinlock global_syslog_rlock;
static pthread_mutex_t global_syslog_wlock = PTHREAD_MUTEX_INITIALIZER;
static YalogUnixSocket *global_syslog_socket;

static YalogUnixSocket *YalogSyslogReconnect() {
  YalogSpinLock(&global_syslog_rlock);
  pthread_mutex_lock(&global_syslog_wlock);
  YalogUnixSocket *old_syslog_socket = global_syslog_socket;
  YalogUnixSocket *new_syslog_socket = NULL;
  new_syslog_socket = YalogOpenUnixSocket(_PATH_LOG, SOCK_DGRAM);
  if (!new_syslog_socket) {
    new_syslog_socket = YalogOpenUnixSocket(_PATH_LOG, SOCK_STREAM);
  }
  global_syslog_socket = YalogAcquireUnixSocket(new_syslog_socket);
  pthread_mutex_unlock(&global_syslog_wlock);
  YalogSpinUnlock(&global_syslog_rlock);
  YalogReleaseUnixSocket(old_syslog_socket);
  return new_syslog_socket;
}

static bool YalogSyslogSendImpl(YalogUnixSocket *syslog_socket,
                                const char *token1, size_t token1_len,
                                const char *token2, size_t token2_len,
                                const char *token3, size_t token3_len,
                                const char *token4, size_t token4_len) {
  if (!syslog_socket) {
    return false;
  }
  struct iovec iov[5];
  int iovcnt = 0;
  if (token1_len > 0) {
    iov[iovcnt].iov_base = (void *)token1;
    iov[iovcnt].iov_len = token1_len;
    ++iovcnt;
  }
  if (token2_len > 0) {
    iov[iovcnt].iov_base = (void *)token2;
    iov[iovcnt].iov_len = token2_len;
    ++iovcnt;
  }
  if (token3_len > 0) {
    iov[iovcnt].iov_base = (void *)token3;
    iov[iovcnt].iov_len = token3_len;
    ++iovcnt;
  }
  if (token4_len > 0) {
    iov[iovcnt].iov_base = (void *)token4;
    iov[iovcnt].iov_len = token4_len;
    ++iovcnt;
  }
  if (YalogGetUnixSocketType(syslog_socket) == SOCK_STREAM) {
    iov[iovcnt].iov_base = "\0";
    iov[iovcnt].iov_len = 1;
    ++iovcnt;
  }
  const ssize_t n = YalogUnixSocketSend(syslog_socket, iov, iovcnt);
  YalogReleaseUnixSocket(syslog_socket);
  return n != -1;
}

void YalogSyslogSend(const char *t1, size_t t1l, const char *t2, size_t t2l,
                     const char *t3, size_t t3l, const char *t4, size_t t4l) {
  YalogUnixSocket *syslog_socket = NULL;
  YalogSpinLock(&global_syslog_rlock);
  syslog_socket = YalogAcquireUnixSocket(global_syslog_socket);
  YalogSpinUnlock(&global_syslog_rlock);
  // first try
  if (YalogSyslogSendImpl(syslog_socket, t1, t1l, t2, t2l, t3, t3l, t4, t4l)) {
    return;
  }
  // second try
  syslog_socket = YalogSyslogReconnect();
  if (YalogSyslogSendImpl(syslog_socket, t1, t1l, t2, t2l, t3, t3l, t4, t4l)) {
    return;
  }
  // third try
  syslog_socket = YalogSyslogReconnect();
  if (YalogSyslogSendImpl(syslog_socket, t1, t1l, t2, t2l, t3, t3l, t4, t4l)) {
    return;
  }
}
