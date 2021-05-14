#ifndef SOCKET_H
#define SOCKET_H

#include "common_socket.h"
#include "server_encriptor.h"

#define ARGC 3

typedef struct server server_t;

void server_init(server_t* server, const char* port);

/*conecta el server */
int server_connect(server_t* self);

/*Permite escuchar posibles conexiones*/
bool server_listen(server_t* self, struct addrinfo* results);

/* Recibe mensajes del cliente */
ssize_t server_recv(server_t* self, char* msg[]);

ssize_t server_send(server_t* self, char msg[], short int msg_size);

/*Acepta la conexión de un cliente */
void server_accept_client(server_t* self);

/*Cierra el server */
void server_close(server_t* self);
/* Inicializa el programa al servidor recibiendo parámetros de main*/
int program_init_server(const char* puerto, char* pre_key);

/* Corre el servidor recibiendo parámetros de main*/
int accept_client(server_t* server, const char* port);


#endif
