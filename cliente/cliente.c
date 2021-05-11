#include "cliente.h"

typedef struct client{
    const char* hostname;
    const char* puerto;
    socket_t* skt;
}client_t;

void client_create(client_t* self, const char* hostname, const char* puerto){
    self->puerto = puerto;
    self->hostname = hostname;
    socket_init(self->skt);
}

void client_close(client_t* self){
    if (!self) return;
    socket_uninit(self->skt);
}

int client_connect(client_t* self){
    return socket_connect(self->skt, self->hostname, self->puerto);
}

ssize_t client_send(client_t* self, char* msg, size_t msg_size){
    char msg_len[2];
    sprintf(msg_len, "%i", (short int)msg_size);
    ssize_t var = socket_send(self->skt, msg_len, 2);
    if(var < 1) return ERROR;
    return socket_send(self->skt, msg, msg_size);
}

int set_up_cliente(client_t* self, const char* puerto, const char* hostname){
    client_create(self, hostname, puerto);
    if(!client_connect(self)) return ERROR;
    return 0;
}

ssize_t client_recive(client_t* self, char* msg){
    char tam[2];
    socket_receive(self->skt, tam, 2);
    size_t msg_size = (size_t)atoi((char*)tam);

    return socket_receive(self->skt, msg, msg_size);
}

int main(){

    return 0;
}
