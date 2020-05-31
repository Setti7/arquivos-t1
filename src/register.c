#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "register.h"

#define REGISTER_OPERATION short int
#define REGISTER_OVERWRITE -1
#define REGISTER_UPDATE -2

Registro *initRegister() {
    /*
     * Inicializa um registro. Caso use os campos de tamaho variavél, eles devem ser alocados individualmente (e free'd
     * também).
     * */

    return calloc(1, sizeof(Registro));
}

RegistroHeader *initRegisterHeader() {
    /*
     * Inicializa um registro de cabeçalho.
     * */

    RegistroHeader *rh = calloc(1, sizeof(RegistroHeader));
    rh->status = '1';
    return rh;
}

void writeHeaderRegister(FILE *fp, RegistroHeader *rh) {
    /*
     * Escreve o registro de cabeçalho no arquivo.
     * */

    fseek(fp, 0, SEEK_SET);

    fwrite(&rh->status, sizeof(char), 1, fp);
    fwrite(&rh->RRNproxRegistro, sizeof(int), 1, fp);

    fwrite(&rh->numeroRegistrosInseridos, sizeof(int), 1, fp);
    fwrite(&rh->numeroRegistrosRemovidos, sizeof(int), 1, fp);
    fwrite(&rh->numeroRegistrosAtualizados, sizeof(int), 1, fp);

    // fill the rest with trash
    char t[111];
    memset(t, '$', 111);
    fwrite(&t, sizeof(char), 111, fp);
}

void internalWriteRegisterRRN(FILE *fp, Registro *r, int RRN, REGISTER_OPERATION op) {
    /*
     * Função interna para escrever um registro em qualquer posição de RRN.
     *
     * Caso REGISTER_OPERATION seja REGISTER_OVERWRITE, todos os dados do registro serão sobre escrevidos e os campos
     * vagos serão preenchidos com lixo.
     *
     * Caso REGISTER_OPERATION seja REGISTER_UPDATE, os campos serão sobreescrevidos, porém, os campos de tamanho
     * fixo não serão completados com lixo.
     * */

    // define trash
    char t[128];
    memset(t, '$', 128);

    // Go to the RRN position
    fseek(fp, 128 + (RRN * 128), SEEK_SET);

    // Write all fields

    // Write the length of dynamic fields
    fwrite(&r->cidadeMae_size, sizeof(int), 1, fp);
    fwrite(&r->cidadeBebe_size, sizeof(int), 1, fp);

    // Write the dynamic fields
    fwrite(r->cidadeMae, sizeof(char), r->cidadeMae_size, fp);
    fwrite(r->cidadeBebe, sizeof(char), r->cidadeBebe_size, fp);

    // skip the remaining space or fill it with trash depending on the operation
    if (op == REGISTER_UPDATE)
        fseek(fp, 97 - r->cidadeBebe_size - r->cidadeMae_size, SEEK_CUR);
    else if (op == REGISTER_OVERWRITE)
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

}

void addRegister(FILE *fp, Registro *r, RegistroHeader *rh) {
    /*
     * Adiciona um registro no final do arquivo.
     * */

    // Set the register status to 0 (busy)
    rh->status = '0';
    writeHeaderRegister(fp, rh);

    // Go to the next RRN position
    int RRN = rh->RRNproxRegistro;

    internalWriteRegisterRRN(fp, r, RRN, REGISTER_OVERWRITE);

    // Update the header
    rh->status = '1';
    rh->RRNproxRegistro++;
    rh->numeroRegistrosInseridos++;
    writeHeaderRegister(fp, rh);
}

void updateRegister(FILE *fp, int RRN, Registro *r) {
    /*
     * Atualiza o registro em RRN.
     * */

    // Set the register status to 0 (busy)
    RegistroHeader *rh = readRegisterHeader(fp);
    rh->status = '0';
    writeHeaderRegister(fp, rh);

    internalWriteRegisterRRN(fp, r, RRN, REGISTER_UPDATE);

    // Update the header
    rh->status = '1';
    rh->numeroRegistrosAtualizados++;
    writeHeaderRegister(fp, rh);
    free(rh);
}

void deleteRegister(FILE *fp, Registro *r, RegistroHeader *rh) {
    /*
     * Marca o registro como removido.
     * */
}

Registro *readRegister(FILE *fp, int RRN) {
    /*
     * Retorna o registro no RRN especificado ou NULL, caso o registro tenha sido removido.
     * */

    // Go to the next RRN position
    fseek(fp, 128 + (RRN * 128), SEEK_SET);
    Registro *r = initRegister();

    // Read all fields

    fread(&r->cidadeMae_size, sizeof(int), 1, fp);
    fread(&r->cidadeBebe_size, sizeof(int), 1, fp);

    if (r->cidadeMae_size >= 0) {
        // tamanho alocado da string é 1 a mais para caber o \0
        r->cidadeMae = calloc(1, (r->cidadeMae_size + 1) * sizeof(char));
        fread(r->cidadeMae, sizeof(char), r->cidadeMae_size, fp);
    } else {
        // caso seja negativo (-1) o registro foi removido
        free(r);
        return NULL;
    }

    if (r->cidadeBebe_size >= 0) {
        // tamanho alocado da string é 1 a mais para caber o \0
        r->cidadeBebe = calloc(1, (r->cidadeBebe_size + 1) * sizeof(char));
        fread(r->cidadeBebe, sizeof(char), r->cidadeBebe_size, fp);
    } else {
        r->cidadeBebe = "\0";
    }

    // Skip the remaining space of trash
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
    /*
     * Retorna o registro de cabeçalho do arquivo.
     * */

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
    /*
     * Printa o registro para stderr para debugar.
     * */

    fprintf(stderr, "Registro: %s (%d) | %s (%d) | %d | %d | %s | %c | %s | %s |\n", r->cidadeMae, r->cidadeMae_size,
            r->cidadeBebe, r->cidadeBebe_size, r->idNascimento, r->idadeMae,
            r->dataNascimento, r->sexoBebe, r->estadoMae, r->estadoBebe);
}

void freeRegister(Registro **r) {
    /*
     * Free no registro, porém ainda é necessário dar free individualmente nos campos de tamanho variavel se eles foram
     * usados.
     * */

    free(*r);
    *r = NULL;
}