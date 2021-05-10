#ifndef CLIENTE_H
#define CLIENTE_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "socket.h"
#include <errno.h>
#include <netdb.h>

typedef struct client client_t;

void client_create(client_t* self, const char* hostname, const char* puerto);

int client_connect(client_t* self);

void client_close(client_t* self);

ssize_t client_send(client_t* self, unsigned char* message,size_t msg_size);

ssize_t client_recive(client_t* self, unsigned char* msg);

int set_up_cliente(client_t* self, const char* puerto, const char* hostname);

bool correct_args(int argc);

#endif
