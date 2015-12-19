#ifndef __yalog_unix_socket_h__
#define __yalog_unix_socket_h__ 1

#include <sys/uio.h>
#include <sys/socket.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct YalogUnixSocket YalogUnixSocket;

YalogUnixSocket* YalogOpenUnixSocket(const char* path, int socket_type);

YalogUnixSocket* YalogAcquireUnixSocket(YalogUnixSocket* self);

void YalogReleaseUnixSocket(YalogUnixSocket* self);

int YalogGetUnixSocketType(YalogUnixSocket* self);

ssize_t YalogUnixSocketSend(YalogUnixSocket* self, struct iovec const* iov,
                            int iovcnt);

#ifdef __cplusplus
}  // extern "C"
#endif
#endif  //_yalog_socket_h__
