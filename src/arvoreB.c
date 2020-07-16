#include "arvoreB.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Cabecalho *initCabecalho() {
    /*
     * Inicializa um registro de cabeçalho.
     * */

    Cabecalho *c = calloc(1, sizeof(Cabecalho));
    c->status = '1';
    c->raiz = -1;
    c->profundidade = 0;
    c->proxRRN = 0;
    c->numeroChaves = 0;
    return c;
}

void escreveCabecalhoIndice (FILE *fp, Cabecalho *c) {
    /*
     * Escreve o registro de cabeçalho no arquivo.
     * */

    fseek(fp, 0, SEEK_SET);

    fwrite(&c->status, sizeof(char), 1, fp);
    fwrite(&c->raiz, sizeof(int), 1, fp);
    fwrite(&c->profundidade, sizeof(int), 1, fp);
    fwrite(&c->proximoRRN, sizeof(int), 1, fp);
    fwrite(&c->numeroChaves, sizeof(int), 1, fp);

    // fill the rest with trash
    char t[55];
    memset(t, '$', 55);
    fwrite(&t, sizeof(char), 55, fp);
}


