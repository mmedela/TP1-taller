#include "cliente.h"
#include <stdio.h>

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
    char msg_len[3];
    snprintf(msg_len, sizeof(msg_len), "%hi",(short int)msg_size);
    ssize_t var = socket_send(self->skt, msg_len, 2);
    if (var < 1) return ERROR;
    return socket_send(self->skt, msg, msg_size);
}

int set_up_cliente(client_t* self, const char* puerto, const char* hostname){
    client_create(self, hostname, puerto);
    if (client_connect(self) != 0) return ERROR;
    return 0;
}

ssize_t client_recive(client_t* self, char** msg){
    char tam[2];
    if (socket_receive(self->skt, tam, 2)<1)return ERROR;
    short int len;
    memcpy(&len, tam, 2);
    *msg = realloc(*msg, (size_t)len + 1);
    *msg[len + 1] = 0;
    return socket_receive(self->skt, *msg, (size_t)len);
}

FILE* get_file_descriptor(const char* path){
    FILE* fd = fopen(path, "r");
    if (!fd) return stdin;
    return fd;
}

void close_file_descriptor(FILE* fd){
    if (fd != stdin) fclose(fd);
}

void imprimir_linea_encriptada(char* msg){
    printf("%s\n", msg);
}

int iter_lineas(client_t* cliente, FILE* fd){
    char buff_send[MAX_SIZE_MSG];
    memset(buff_send, 0, MAX_SIZE_MSG*size_of_chunck);
    size_t pos = 0;
    while (!feof(fd)){
        char* buff_recive = calloc(MAX_SIZE_MSG, sizeof(char));
        char buff_read[MAX_SIZE_RECIVE];
        if (!fread(buff_read, size_of_chunck, N_CHUNKS, fd)) return ERROR;
        if (buff_read[0] == '\n'){
            pos++;
            if (client_send(cliente, buff_send, pos) == ERROR) return ERROR;
            client_recive(cliente,&buff_recive);
            imprimir_linea_encriptada(buff_recive);
            pos = 0;
            memset(buff_send, 0, MAX_SIZE_MSG*size_of_chunck);
        }else{
            buff_send[pos] = buff_read[0];
            pos++;
        }
        free(buff_recive);
    }
    return 0;
}

int main(int argc, char const *argv[]){
    if (argc < ARGS_CLIENT) return 0;
    client_t client;
    if (set_up_cliente(&client, argv[2], argv[1]) == ERROR) return ERROR;
    FILE* fd = get_file_descriptor(argv[3]);
    if (!fd) return -1;
    if (iter_lineas(&client, fd) != ERROR) return ERROR;
    close_file_descriptor(fd);
    client_close(&client);
    return 0;
}












