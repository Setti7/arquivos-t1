#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "register.h"


Registro *initRegister() {
    return calloc(1, sizeof(Registro));
}

RegistroHeader *initRegisterHeader() {
    RegistroHeader *rh = calloc(1, sizeof(RegistroHeader));
    rh->status = '1';
    return rh;
}

void writeHeaderRegister(FILE *fp, RegistroHeader *rh) {
    fseek(fp, 0, SEEK_SET);

    fwrite(&rh->status, sizeof(char), 1, fp);
    fwrite(&rh->RRNproxRegistro, sizeof(int), 1, fp);

    fwrite(&rh->numeroRegistrosInseridos, sizeof(int), 1, fp);
    fwrite(&rh->numeroRegistrosRemovidos, sizeof(int), 1, fp);
    fwrite(&rh->numeroRegistrosAtualizados, sizeof(int), 1, fp);

    // fill the rest with $
    char t[111];
    memset(t, '$', 111);
    fwrite(&t, sizeof(char), 111, fp);
}

void addRegister(FILE *fp, Registro *r, RegistroHeader *rh) {
    /*
     * TODO: Ainda é necessário tratar nulos. Apenas o idNascimento é não-nulo.
     *
     * TODO:                        1 arq02.csv out.bin
     *
     * Todos os campos nulos string FIXOS devem ser do tipo \0$$$$.... até completar o tamanho do campo.
     * Todos os campos nulos int/float/double FIXOS devem ser do tipo -1.
     * */

    // trash
    char t[128];
    memset(t, '$', 128);

    // Set the register status to 0 (busy)
    rh->status = '0';
    writeHeaderRegister(fp, rh);

    // Go to the next RRN position
    int RRN = rh->RRNproxRegistro;
    fseek(fp, 128 + (RRN * 128), SEEK_SET);

    // Write all fields

    // Write the length of dynamic fields
    fwrite(&r->cidadeMae_size, sizeof(int), 1, fp);
    fwrite(&r->cidadeBebe_size, sizeof(int), 1, fp);

    // Write the dynamic fields
    fwrite(r->cidadeMae, sizeof(char), r->cidadeMae_size, fp);
    fwrite(r->cidadeBebe, sizeof(char), r->cidadeBebe_size, fp);

    // fill the rest with trash
    fwrite(&t, sizeof(char), 97 - r->cidadeBebe_size - r->cidadeMae_size, fp);

    // idNascimento não precisa tratar pois nunca é nulo
    fwrite(&r->idNascimento, sizeof(int), 1, fp);

    // Tratamento da idadeMae
    if (r->idadeMae <= 0) {
        int idadeMaeInvalida = -1;
        fwrite(&idadeMaeInvalida, sizeof(int), 1, fp);
    } else {
        fwrite(&r->idadeMae, sizeof(int), 1, fp);
    }

    // Tratamento da data de nascimento
    if (strlen(r->dataNascimento) == 0) {
        t[0] = '\0';
        fwrite(&t, sizeof(char), 10, fp);
        memset(t, '$', 128);
    } else {
        fwrite(&r->dataNascimento, sizeof(char), 10, fp);
    }

    fwrite(&r->sexoBebe, sizeof(char), 1, fp);

    // Tratamento estadoMae
    if (strlen(r->estadoMae) == 0) {
        t[0] = '\0';
        fwrite(&t, sizeof(char), 2, fp);
        memset(t, '$', 128);
    } else {
        fwrite(&r->estadoMae, sizeof(char), 2, fp);
    }

    // Tratamento estadoBebe
    if (strlen(r->estadoBebe) == 0) {
        t[0] = '\0';
        fwrite(&t, sizeof(char), 2, fp);
        memset(t, '$', 128);
    } else {
        fwrite(&r->estadoBebe, sizeof(char), 2, fp);
    }

    // Update the header
    rh->status = '1';
    rh->RRNproxRegistro++;
    rh->numeroRegistrosInseridos++;
    writeHeaderRegister(fp, rh);
}

void updateRegister(FILE *fp, Registro *r, RegistroHeader *rh) {}

void deleteRegister(FILE *fp, Registro *r, RegistroHeader *rh) {}

Registro *readRegister(FILE *fp, int RRN) {
    // Go to the next RRN position
    fseek(fp, 128 + (RRN * 128), SEEK_SET);
    Registro *r = initRegister();

    fread(&r->cidadeMae_size, sizeof(int), 1, fp);
    fread(&r->cidadeBebe_size, sizeof(int), 1, fp);


    if (r->cidadeMae_size > 0) {
        // tamanho alocado da string é 1 a mais para caber o \0
        r->cidadeMae = calloc(1, (r->cidadeMae_size + 1) * sizeof(char));
        fread(r->cidadeMae, sizeof(char), r->cidadeMae_size, fp);
    } else {
        r->cidadeMae = "\0";
    }

    if (r->cidadeBebe_size > 0) {
        // tamanho alocado da string é 1 a mais para caber o \0
        r->cidadeBebe = calloc(1, (r->cidadeBebe_size + 1) * sizeof(char));
        fread(r->cidadeBebe, sizeof(char), r->cidadeBebe_size, fp);
    } else {
        r->cidadeBebe = "\0";
    }


    fseek(fp, 97 - r->cidadeBebe_size - r->cidadeMae_size, SEEK_CUR);

    fread(&r->idNascimento, sizeof(int), 1, fp);
    fread(&r->idadeMae, sizeof(int), 1, fp);

    fread(&r->dataNascimento, sizeof(char), 10, fp);
    fread(&r->sexoBebe, sizeof(char), 1, fp);
    fread(&r->estadoMae, sizeof(char), 2, fp);
    fread(&r->estadoBebe, sizeof(char), 2, fp);

    return r;
}

RegistroHeader *readRegisterHeader(FILE *fp) {
    fseek(fp, 0, SEEK_SET);

    RegistroHeader *rh = initRegisterHeader();

    fread(&rh->status, sizeof(char), 1, fp);
    fread(&rh->RRNproxRegistro, sizeof(int), 1, fp);
    fread(&rh->numeroRegistrosInseridos, sizeof(int), 1, fp);
    fread(&rh->numeroRegistrosRemovidos, sizeof(int), 1, fp);
    fread(&rh->numeroRegistrosAtualizados, sizeof(int), 1, fp);

    return rh;
}

void printRegister(Registro *r) {
    fprintf(stderr, "Registro: %s (%d) | %s (%d) | %d | %d | %s | %c | %s | %s |\n", r->cidadeMae, r->cidadeMae_size,
            r->cidadeBebe, r->cidadeBebe_size, r->idNascimento, r->idadeMae,
            r->dataNascimento, r->sexoBebe, r->estadoMae, r->estadoBebe);
}

void freeRegister(Registro **r) {
    if ((*r)->cidadeMae_size > 0) free((*r)->cidadeMae);
    if ((*r)->cidadeBebe_size > 0) free((*r)->cidadeBebe);
    free(*r);
    *r = NULL;
}
