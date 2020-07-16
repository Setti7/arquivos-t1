#ifndef ARVOREB_H_INCLUDED
#define ARVOREB_H_INCLUDED

#include <stdio.h>

#define MAX_KEYS 5  //numero de chaves por pagina definido pela especificacao

struct arvoreBPagina {
    int nivel;                  //informa o nivel de profundidade da pagina
    int numeroDeChaves;         //contador de ocupacao da pagina
    int chaves[MAX_KEYS];       //as chaves
    int RRNChaves[MAX_KEYS];    //RRN dos campos no arquivo de dados
    int filhos[MAX_KEYS+1];     //RRN das paginas filhas
} Pagina;

struct chave {
    int valor;
    int RRN;
} Chave;

struct arvoreBCabecalho {
    char status;        //informa a consistencia do arquivo de indices
    int raiz;           //informa o RRN do no raiz
    int profundidade;   //numero de niveis da arvore
    int proxRRN;        //informa qual a proxima pagina livre para inserir elementos na arvore
    int numeroChaves;   //informa quantas chaves estao armazenadas no arquivo
    //char lixo[55];      //lixo para preencher o valor de 72 bytes do cabecalho
} Cabecalho;

void escreveCabecalhoIndice (FILE *fp, Cabecalho *c);

int busca(FILE *fp, int RRN, int chave, int *foundRRN, int *foundPOS);

#endif // ARVOREB_H_INCLUDED
