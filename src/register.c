#include <stdio.h>
#include <string.h>
#include "register.h"

void writeHeaderRegister(FILE *fp, RegistroHeader *rh) {
    fseek(fp, 0, SEEK_SET);

    fwrite(&rh->status, sizeof(char), 1, fp);
    fwrite(&rh->RRNproxRegistro, sizeof(int), 1, fp);

    fwrite(&rh->numeroRegistrosInseridos, sizeof(int), 1, fp);
    fwrite(&rh->numeroRegistrosRemovidos, sizeof(int), 1, fp);
    fwrite(&rh->numeroRegistrosAtualizados, sizeof(int), 1, fp);

    char t[111];
    memset(t, '$', 111);
    fwrite(&t, sizeof(char), 111, fp);

    fseek(fp, 0, SEEK_END);
}

void addRegister(FILE *fp, Registro *r, RegistroHeader *rh) {
    // Set the register status to 0
    rh->status = 0;
    writeHeaderRegister(fp, rh);

    // Go to the next RRN position
    int RRN = rh->RRNproxRegistro;
    fseek(fp, SEEK_SET, 128 + (RRN * 128));

    // Write all fields

    // Write the lenght of dynamic fields
    fwrite(&r->cidadeMae_size, sizeof(int), 1, fp);
    fwrite(&r->cidadeBebe_size, sizeof(int), 1, fp);

    // Write the dynamic fields
    fwrite(r->cidadeMae, sizeof(char), r->cidadeMae_size, fp);
    fwrite(r->cidadeBebe, sizeof(char), r->cidadeBebe_size, fp);

    // Skip the rest of the dynamic fields
    fseek(fp, 99 - r->cidadeBebe_size - r->cidadeMae_size, SEEK_CUR);


    fwrite(&r->idNascimento, sizeof(int), 1, fp);
    fwrite(&r->idadeMae, sizeof(int), 1, fp);

    fwrite(&r->dataNascimento, sizeof(char), 10, fp); // todo: tratar nulo

    fwrite(&r->sexoBebe, sizeof(char), 1, fp);

    fwrite(&r->estadoMae, sizeof(char), 2, fp);
    fwrite(&r->estadoBebe, sizeof(char), 2, fp);

    // Update the header
    rh->status = 1;
    rh->RRNproxRegistro++;
    rh->numeroRegistrosInseridos++;
    writeHeaderRegister(fp, rh);
}

void updateRegister(FILE *fp, Registro *r, RegistroHeader *rh) {

}

void deleteRegister(FILE *fp, Registro *r, RegistroHeader *rh) {

}

void printRegister(Registro *r) {
    printf("Registro: %s (%d) | %s (%d) | %d | %d | %s | %d | %s | %s |", r->cidadeMae, r->cidadeMae_size,
           r->cidadeBebe, r->cidadeBebe_size, r->idNascimento, r->idadeMae,
           r->dataNascimento, r->sexoBebe, r->estadoMae, r->estadoBebe);
}