#ifndef ENCRIPTOR_H_INCLUDED
#define ENCRIPTOR_H_INCLUDED

typedef struct encriptor{

    int** matriz;
    int curr_num_line_len;
    size_t lado;
}encriptor_t;

void init_encriptor(char key[], encriptor_t* self);

void delete_encriptor(encriptor_t* self);

char* encriptar_linea(encriptor_t* self, char linea[]);


#endif // ENCRIPTOR_H_INCLUDED
