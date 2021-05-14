#ifndef SOCKET_H_INCLUDED
#define SOCKET_H_INCLUDED

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define MAX_CLIENTS 1
#define REQUEST_MAX_LEN  1024
#define RESPONSE_MAX_LEN 1024
#define ERROR              -1

typedef struct addrinfo addrinfo_t;

typedef struct socket {
    int fd;
} socket_t;

void socket_init(socket_t *self);

void socket_uninit(socket_t *self);

bool socket_bind(socket_t* self, 
	struct sockaddr* ai_addr, socklen_t ai_addrlen);

bool socket_listen(socket_t* self, int cant);

void socket_accept(socket_t *listener, socket_t *peer);

int socket_connect(socket_t *self, const char *host, const char *service);

ssize_t socket_send(socket_t* self, char buffer[], size_t length);

ssize_t socket_receive(socket_t* self, char msg[], size_t msg_size);


#endif // SOCKET_H_INCLUDED
