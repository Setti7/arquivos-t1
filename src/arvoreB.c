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

int busca(FILE *fp, int RRN, int chave, int *foundRRN, int *foundPOS) {
    if(RRN == -1) {
        //nao encontrou
        return -1;
    }

    Pagina p;
    //READ PAGE
    for(int i = 0; i<p.numeroDeChaves; i++){
        if (chave < p.chaves[i]) {
            return busca (fp, p.filhos[i], chave, foundRRN, foundPOS);
        }
        if (chave == p.chaves[i]){
            //achou!
            &foundRRN = RRN;
            &foundPOS = i;
            return 1;
        }
    }
    if(i<MAX_KEYS) {
        &foundRRN = RRN;
        &foundPOS = i;
        return 0; //nao achou a chave mas achou onde inserir ela.
    }
    return busca (fp, p.filhos[i], chave, foundRRN, foundPOS);

}
