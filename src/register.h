#ifndef T1_REGISTER_H
#define T1_REGISTER_H

#include <stdio.h>

struct registroHeader {
    char status;
    int RRNproxRegistro;
    int numeroRegistrosInseridos;
    int numeroRegistrosRemovidos;
    int numeroRegistrosAtualizados;
};

struct registro {
    int cidadeMae_size;
    int cidadeBebe_size;
    char *cidadeMae;
    char *cidadeBebe;
    int idNascimento;
    int idadeMae;
    char dataNascimento[11];
    int sexoBebe;
    char estadoMae[3];
    char estadoBebe[3];
};

typedef struct registroHeader RegistroHeader;
typedef struct registro Registro;

void writeHeaderRegister(FILE *fp, RegistroHeader *rh);

void addRegister(FILE *fp, Registro *r, RegistroHeader *rh);

void updateRegister(FILE *fp, Registro *r, RegistroHeader *rh);

void deleteRegister(FILE *fp, Registro *r, RegistroHeader *rh);

void printRegister(Registro *r);

Registro *initRegister();

RegistroHeader *initRegisterHeader();

#endif //T1_REGISTER_H
