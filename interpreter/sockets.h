#ifndef SOCKETS_H
#define SOCKETS_H

// include headers based on OS
#if defined WIN32
#include <winsock.h>  // WinSock subsystem
#include <winsock2.h>  // WinSock subsystem
#elif defined __linux__
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <string.h>

// redefine some types and constants based on OS
#if defined WIN32
typedef int socklen_t;  // Unix socket length
#define bzero(ptr,n) memset(ptr, 0, n)
#define bcopy(b1, b2, len) (memmove((b2),(b1),(len)),(void) 0)
#elif defined __linux__
typedef int SOCKET;
#define INVALID_SOCKET -1  // WinSock invalid socket
#define SOCKET_ERROR   -1  // basic WinSock error
#define closesocket(s) close(s);  // Unix uses file descriptors, WinSock doesn't...
#endif

#include "../head/comm.h"
#include "debug.h"

void socket_setup();
SOCKET socket_connect();
void socket_disconnect(SOCKET);
void socket_teardown();
int send_and_select_response(SOCKET,MESSAGE*,MESSAGE*);
int send_message(SOCKET, MESSAGE*);
#endif
