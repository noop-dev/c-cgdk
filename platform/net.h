#ifndef _NOOP_NET_H_
#define _NOOP_NET_H_

/*****************************************************************************************
* Minimal wrapper for TCP/IP sockets V1.1 (C) by Boris Chuprin 2013
* free for use for any purpose, but I refuse to be held responsible for any outcome
*****************************************************************************************/

#if defined(_MSC_VER)
/*
 * M$ Visual C++ Compiler
 */
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;
typedef unsigned __int64 uint64;
typedef signed __int64 int64;
#define NOMINMAX
#else
/*
 * All other modern C compilers.
 */
#include <stddef.h>
#include <stdint.h>

typedef uint16_t uint16;
typedef int16_t int16;
typedef uint32_t uint32;
typedef int32_t int32;
typedef uint64_t uint64;
typedef int64_t int64;

#endif

#include <stdlib.h>

#if defined(WIN32)
#include <WinSock2.h>

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

/*
 * Create new network socket. Returns INVALID_SOCKET on failure
 */
SOCKET socket_init_tcp();

/*
 * Create new network socket. Returns INVALID_SOCKET on failure
 */
SOCKET socket_init_udp();

/*
 * Close the socket. Returns 0 on success, platform-dependent error code otherwise
 */
int socket_close(SOCKET socket);

/*
 * Use non-zero parameter to enable, 0 to disable nagle algorithm for a socket. Returns 0 on success.
 */
int socket_enable_nagle_algorithm(SOCKET socket, int enable);

/*
 * Conect socket to the specified address. Returns 0 on success.
 */
int socket_connect(SOCKET socket, const char *host_name, unsigned port);

/*
 * Receive specified number of bytes. Blocking call. Returns 0 on success, non-zero on error or disconnect
 */
int socket_recv(SOCKET socket, char *data, size_t size);

/*
 * Send specified number of bytes. Blocking call. Returns 0 on success, non-zero on error or disconnect
 */
int socket_send(SOCKET socket, const char *data, size_t size);

#endif /* _NOOP_NET_H_ */
