#ifndef _NOOP_NET_H_
#define _NOOP_NET_H_

/*****************************************************************************************
* Minimal wrapper for TCP/IP sockets (C) by Boris Chuprin 2013
* free for use for any purpose, but I refuse to be held responsible for any outcome
*****************************************************************************************/

#include <stdint.h>
#include <stddef.h>

#if defined(WIN32)

#include <WinSock2.h>
#include <ws2ipdef.h>

typedef int socklen_t;
#define socket_last_error()                                WSAGetLastError()

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

typedef int SOCKET;
#define INVALID_SOCKET                                   -1

#define socket_last_error()                              errno
#endif

/* Returns INVALID_SOCKET on failure */
SOCKET socket_init_tcp();

/* Returns INVALID_SOCKET on failure */
SOCKET socket_init_udp();

/* Returns 0 on success, platform-dependent error code otherwise */
int socket_close(SOCKET socket);

/* Use non-zero parameter to enable, 0 to disable nagle algorithm for a socket. returns 0 on success. */
int socket_enable_nagle_algorithm(SOCKET socket, int enable);

int socket_connect(SOCKET socket, const char *host_name, unsigned port);

/* Receive specified number of bytes. Blocking call. Returns 0 on success, non-zero on error or disconnect */
int socket_recv(SOCKET socket, char *data, unsigned long size);

/* Send specified number of bytes. Blocking call. Returns 0 on success, non-zero on error or disconnect */
int socket_send(SOCKET socket, const char *data, unsigned long size);

#endif /* __NOOP_NETWORK__ */