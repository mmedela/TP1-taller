#ifndef CLIENTE_H
#define CLIENTE_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "common_socket.h"
#include <errno.h>
#include <netdb.h>
#define ARGS_CLIENT        3
#define N_CHUNKS           1
#define MAX_SIZE_RECIVE    3
#define MAX_SIZE_MSG    1024

const size_t size_of_chunck = sizeof(char);

typedef struct client client_t;

void client_create(client_t* self, const char* hostname, const char* puerto);

int client_connect(client_t* self);

void client_close(client_t* self);

ssize_t client_send(client_t* self, char* message,size_t msg_size);

ssize_t client_recive(client_t* self, char** msg);

int set_up_cliente(client_t* self, const char* puerto, const char* hostname);

bool correct_args(int argc);

#endif
