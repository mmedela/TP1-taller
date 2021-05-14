#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "server_encriptor.h"
#define LARGO_ABECEDARIO 26

const char ABECEDARIO[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

typedef struct encriptor encriptor_t;

int to_number(char c){
    for (int i = 0; i < LARGO_ABECEDARIO; i++){
        if (c == ABECEDARIO[i]) return i;
    }
    return 0;
}

int get_largo_linea(encriptor_t* self){
    return self->curr_num_line_len;
}

char to_char(int num){
    while (num > LARGO_ABECEDARIO){
        num -= LARGO_ABECEDARIO;
    }
    return ABECEDARIO[num];
}

void set_array_en_cero(size_t lado, int arr[]){
    for (size_t i = 0; i < lado; i++){
        arr[i] = 0;
    }
}

void create_new_matriz(encriptor_t* self){
    int** new_matriz = calloc(self->lado, sizeof(int*));
    for (int i = 0; i < self->lado; i++){
        new_matriz[i] = calloc(self->lado, sizeof(int));
    }
    self->matriz = new_matriz;
}

void set_matriz(encriptor_t* self, char key[]){
    create_new_matriz(self);
    int pos = 0;
    for (int i = 0; i < self->lado; i++){
        for (int j = 0; j < self->lado; j++){
            self->matriz[i][j] = to_number(key[pos]);
            pos++;
        }
    }
}

void init_encriptor(char key[], encriptor_t* self){
    switch (strlen(key)){
    case 4:
        self->lado = 2;
        set_matriz(self, key);
        break;
    case 9:
        self->lado = 3;
        set_matriz(self, key);
        break;
    case 16:
        self->lado = 4;
        set_matriz(self, key);
        break;
    }
}

void delete_encriptor(encriptor_t* self){
    for (int i = 0; i < self->lado; i++){
        free(self->matriz[i]);
    }
    free(self->matriz);
}

void fill_number_line(encriptor_t* self, char current_line[],
		int number_line[], size_t largo1, size_t largo2){
    for (int i = 0; i < largo1; i++){
        number_line[i] = to_number(current_line[i]);
    }
}

void multiplicar_matrices(encriptor_t* self, int num_line[]){
    int vec1[self->lado], vec2[self->lado];
    int pos_num_line = 0, last_changed_pos = 0;
    set_array_en_cero(self->lado, vec2);
    bool end = false;
    while (!end){
        for (size_t i = 0; i<self->lado; i++){
            vec1[i] = num_line[pos_num_line];
            pos_num_line++;
        }
        for (size_t i = 0; i < self->lado; i++){
            for (size_t j = 0; j<self->lado; j++){
                vec2[i] += vec1[j] * (self->matriz[i][j]);
            }
        }
        for (size_t i = 0; i < self->lado; i++){
            num_line[last_changed_pos] = vec2[i];
            last_changed_pos++;
        }
        set_array_en_cero(self->lado, vec2);
        if (pos_num_line == self->curr_num_line_len) end = true;
    }
}

void fill_respuesta(char respuesta[], int num_line[], size_t largo){
    for (size_t i = 0; i < largo; i++){
        respuesta[i] = to_char(num_line[i]);
    }
}

char* encriptar_linea(encriptor_t* self, char linea[]){
    size_t largo = strlen(linea), aux = 0;
    size_t resto = largo % self->lado;
    if (resto != 0) aux = largo + resto;

    int* num_line = malloc(aux*sizeof(int));

    self->curr_num_line_len = aux;
    set_array_en_cero(aux, num_line);
    fill_number_line(self, linea, num_line, largo, aux);
    multiplicar_matrices(self, num_line);
    char* respuesta = calloc(aux+1, sizeof(char));
    if (!respuesta) return NULL;
    fill_respuesta(respuesta, num_line, aux);
    free(num_line);
    return respuesta;
}
