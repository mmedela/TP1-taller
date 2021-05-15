
#include "server.h"
#include "common_socket.h"
#define MAX_CLIENTS 1

struct server{
    const char* puerto;
    socket_t* skt;
    socket_t* client_skt;
};


void server_init(server_t* self, const char* puerto){
    self->puerto = puerto;
}

int server_connect(server_t* self){
    if (socket_connect(self->skt, NULL, self->puerto) < 0) return ERROR;
    return 0;
}

void server_accept_client(server_t* self){
    socket_accept(self->skt, self->client_skt);
}

ssize_t server_send(server_t* self, char msg[], short int length){
    return socket_send(self->skt, msg, length);
}

ssize_t server_recv(server_t* self, char* msg[]){
    *msg = calloc(RESPONSE_MAX_LEN, sizeof(char));
    return socket_receive(self->client_skt, *msg);
}


void server_uninit(server_t* self){
    socket_uninit(self->skt);
    socket_uninit(self->client_skt);
}



int accept_client(server_t* self, const char* puerto ){
    server_init(self, puerto);
    if ( server_connect(self) < 0 ){
        server_uninit(self);
        return ERROR;
    }
    server_accept_client(self);
    return 0;
}



int program_init_server(const char* puerto, char* pre_key){
    server_t server;
    socket_t client_skt, socket;
    server.skt = &socket;
    server.client_skt = &client_skt;
    encriptor_t enc;
    if (accept_client(&server, puerto ) < 0) return ERROR;
    init_encriptor(pre_key, &enc);
    char *msg = 0;
    while (true){
    	char* aux;
        if (server_recv(&server, &msg) == ERROR)break;
        aux = encriptar_linea(&enc, msg);
        if (!aux) continue;
        short int l = (short int)get_largo_linea(&enc);
        if (server_send(&server,aux, l)==ERROR)break;
        free(aux);
    }
    server_uninit(&server);
    delete_encriptor(&enc);
    free(msg);
    return 0;
}

int main(int argc, char* argv[]){
    if (argc != ARGC) return ERROR;
    const char* puerto = argv[1];
    return program_init_server(puerto, argv[2]);
}
