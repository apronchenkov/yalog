#include "unix_socket.h"
#include <errno.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

struct YalogUnixSocket {
  atomic_uint ref_counter;
  int socket_type;
  int socket;
};

static inline int Connect(int socket, const void* address,
                          socklen_t address_len) {
  int result;
  do {
    result = connect(socket, (const struct sockaddr*)address, address_len);
  } while (result == -1 && errno == EINTR);
  return result;
}

static inline void Close(int socket) {
  int result;
  do {
    result = close(socket);
  } while (result == -1 && errno == EINTR);
  if (result == -1) {
    /* second try */
    do {
      result = close(socket);
    } while (result == -1 && errno == EINTR);
  }
}

YalogUnixSocket* YalogOpenUnixSocket(const char* path, int socket_type) {
  YalogUnixSocket* result = NULL;
  int sock = -1;
  struct sockaddr_un sockaddr_un;
  if (sizeof(sockaddr_un.sun_path) <= strlen(path)) {
    errno = ENAMETOOLONG;
    goto on_fail;
  }
  memset(&sockaddr_un, 0, sizeof(sockaddr_un));
  sockaddr_un.sun_family = AF_UNIX;
  strcpy(sockaddr_un.sun_path, path);
  sock = socket(AF_UNIX, socket_type, 0);
  if (sock == -1) {
    goto on_fail;
  }
  if (Connect(sock, &sockaddr_un, sizeof(sockaddr_un)) == -1) {
    goto on_fail;
  }
  result = malloc(sizeof(YalogUnixSocket));
  if (result == NULL) {
    goto on_fail;
  }
  result->ref_counter = 1;
  result->socket_type = socket_type;
  result->socket = sock;
  return result;
on_fail:
  if (sock != -1) {
    int errno_copy = errno;
    Close(sock);
    errno = errno_copy;
  }
  return NULL;
}

YalogUnixSocket* YalogAcquireUnixSocket(YalogUnixSocket* self) {
  if (self) {
    atomic_fetch_add_explicit(&self->ref_counter, 1, memory_order_relaxed);
  }
  return self;
}

void YalogReleaseUnixSocket(YalogUnixSocket* self) {
  if (self &&
      atomic_fetch_sub_explicit(&self->ref_counter, 1, memory_order_relaxed) ==
          0) {
    Close(self->socket);
    free(self);
  }
}

int YalogGetUnixSocketType(YalogUnixSocket* self) { return self->socket_type; }

ssize_t YalogUnixSocketSend(YalogUnixSocket* self, struct iovec const* iov,
                            int iovcnt) {
  struct msghdr message;
  memset(&message, 0, sizeof(message));
  message.msg_iov = (struct iovec*)iov;
  message.msg_iovlen = iovcnt;
  ssize_t result;
  do {
    result = sendmsg(self->socket, &message, 0);
  } while (result == -1 && errno == EINTR);
  return result;
}
