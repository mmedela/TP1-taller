
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

ssize_t server_send(server_t* self, char msg[], short int msg_size){
    char msg_len[2];
    memcpy(msg_len, &msg_size, 2);
    if(socket_send(self->skt, msg_len, 2) < 1) return ERROR;
    return socket_send(self->skt, msg, msg_size);
}

ssize_t server_recv(server_t* self, char* msg[]){
    char msg_size[2];
    if(socket_receive(self->client_skt, msg_size, 2) < 1) return ERROR;
    short int len;
    memcpy(&len, msg_size, 2);
    *msg = realloc(*msg, (size_t)len + 1);
    *msg[len + 1] = 0;
    return socket_receive(self->client_skt, *msg, (size_t)len);
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



int program_init_server(char* pre_key, const char* puerto){
    server_t server;
    socket_t client_skt, socket;
    server.skt = &socket;
    server.client_skt = &client_skt;
    encriptor_t enc;

    if (accept_client(&server, puerto ) < 0) return ERROR;
    init_encriptor(pre_key, &enc);
    bool seguir = true;
    char *msg = 0, *aux = 0;
    while(seguir){
        short int len = (short int)server_recv(&server, &msg);
        if(len == 0)break;
        aux = encriptar_linea(&enc, msg);
        if(!aux) continue;
        if(server_send(&server, aux, len) == 0) break;
        free(aux);
    }
    server_uninit(&server);
    delete_encriptor(&enc);
    free(msg);
    return 0;
}

int main(){

    return 0;
}
