
#include "common_socket.h"



void socket_init(socket_t* self){
    self->fd = -1;
}

int socket_create(socket_t* self, struct addrinfo* ptr){
    self->fd = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol);
    if (self->fd == -1){
        printf("ERROR asignando socket \n");
        socket_uninit(self);
        return -1;
    }
    int peer = connect(self->fd, ptr->ai_addr, ptr->ai_addrlen);
    if (peer == -1){
        printf("Error de conexión de socket: %s \n", strerror(errno));
        socket_uninit(self);
        return -1;
    }
    return peer;
}

void socket_uninit(socket_t* self){
    if (!self) return;
    if (shutdown(self->fd, SHUT_RDWR) == -1){
        printf("Error: %s", strerror(errno));
    }
    if (close(self->fd) == -1){
        printf("Error: %s", strerror(errno));
    }
}

int socket_config(socket_t* self, bool serv_flag,const char* port,  const char* host, struct addrinfo** results){

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = serv_flag ? AI_PASSIVE : 0;
    if (getaddrinfo(host, port, &hints, results) != 0){
        fprintf(stderr,"Error obteniendo informacion getaddrinfo.\n");
        return -1;
    }
    return 0;
}

int client_iterate_results(socket_t* self, addrinfo_t* result){
    struct addrinfo* ptr;
    bool is_connected = false;

    for (ptr = result; ptr != NULL && is_connected == false; ptr = ptr->ai_next){
        int peer = socket_create(self, ptr);
        if (peer == -1) {
            continue;
        }
        is_connected = (peer != -1);
   }
   return is_connected;
}

int connect_as_client(socket_t* self, const char* host, const char* port){
    int err = 0;
    struct addrinfo* result;
    if (socket_config(self, false, port, host, &result) < 0){
        fprintf(stderr,"Error obteniendo informacion getaddrinfo.\n");
        err = ERROR;
    }
    if (client_iterate_results(self, result) == -1){
        printf("Error en resultados");
        err = ERROR;
    }
    freeaddrinfo(result);
    return err;
}

int connect_as_server(socket_t* self, const char* port){
    struct addrinfo* results;
    struct addrinfo* ptr;
    bool is_connected = false;
    if (socket_config(self, true, port, NULL, &results) < 0){
        fprintf(stderr,"Error obteniendo informacion getaddrinfo.\n");
        return ERROR;
    }
    for (ptr = results; ptr != NULL && is_connected == false; ptr = ptr->ai_next){

        socket_create(self, ptr);
        if (!socket_bind(self, results->ai_addr, results->ai_addrlen))
            return ERROR;
        if (!socket_listen(self, MAX_CLIENTS))
            return ERROR;
        is_connected = true;
    }
    freeaddrinfo(results);
    return 0;
}

int socket_connect(socket_t* self, const char* host, const char* port){

    if(host) return connect_as_client(self, host, port);
    return connect_as_server(self, port);
}

bool socket_bind(socket_t* self, struct sockaddr* ai_addr, socklen_t ai_addrlen){

    int option = 1;
    if (setsockopt(self->fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) == -1){

        fprintf(stderr,"Error setsock: %s\n",strerror(errno));
        socket_uninit(self);
        return false;
    }
    int err = bind(self->fd, ai_addr, ai_addrlen);
    if (err == -1){
        printf("Error bind: %s \n", strerror(errno));
        socket_uninit(self);
        return false;
    }
    return true;
}

bool socket_listen(socket_t* self, int cant){
    int err = listen(self->fd, cant);
    if (err == -1){
        printf("Error listen: %s \n", strerror(errno));
        return false;
    }
    return true;
}

void socket_accept(socket_t *listener, socket_t *peer){
    peer->fd = accept(listener->fd, NULL, NULL);
    if (peer->fd == -1){
        fprintf(stderr, "Error fd accept: %s \n", strerror(errno));
    }
}

ssize_t socket_send(socket_t* self, char buffer[], size_t length){
    if (length == 0) return 0;
    int tot_bytes = 0;
    while (tot_bytes < length){
        ssize_t bytes = send(self->fd, &buffer[tot_bytes], length - tot_bytes, MSG_NOSIGNAL);
        if (bytes == -1) {
            fprintf(stderr, "Error mandando mensaje: %s \n", strerror(errno));
            return -1;
        }
        if (bytes == 0)
            return 0;
        else
            tot_bytes += bytes;
    }
    return tot_bytes;
}

ssize_t socket_receive(socket_t* self, char buffer[], size_t length){
    if (length == 0) return 0;
    ssize_t tot_bytes = 0;
    ssize_t new_bytes = 0;

    while (new_bytes < length){
        new_bytes = recv(self->fd, &buffer[tot_bytes], length - tot_bytes, 0);
        if (new_bytes == -1){
            fprintf(stderr, "Error del receive new bytes: %s \n",strerror(errno));
            return ERROR;
        }else if (new_bytes == 0){
            return tot_bytes;
        }else if (new_bytes < REQUEST_MAX_LEN){
            return new_bytes;
        }
        tot_bytes += new_bytes;
    }
    return tot_bytes;
}

