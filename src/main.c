/*
 * Organiza��o de Arquivos - T1
 *
 * Feito por:
 *  Andr� Niero Setti - 10883901
 *  Leonardo Chieppe  - 9368730
 *
 * */
#include "register.h"
#include <stdio.h>
#include <stdlib.h>
#include "binarioNaTela.h"
#include <string.h>

void firstOperation(char* arquivoEntrada, char* arquivoSaida);

void printOptions();

void Funcionalidade2(char *nomeArquivo);

int main() {

    FILE *fp = fopen("out.bin", "wb"); //wb para escrever em binario
    int operation = 0;

    while (1) {
        printOptions(); //lembrar de comentar antes de enviar
        scanf("%d", &operation);

        if (operation == 1) {
            char arquivoEntrada[100], arquivoSaida[100];
            scanf("%s %s", arquivoEntrada, arquivoSaida);
            firstOperation(arquivoEntrada, arquivoSaida);
        } else if (operation == 2) {
            char nomeArquivo[100];
            scanf("%s", nomeArquivo);
            Funcionalidade2(nomeArquivo);
        } else if (operation == 3) {
            break;
        } else {
            printf("Opera��o inv�lida.\n"); //lembrar de comentar antes de enviar
        }
    }

    printf("Saindo..."); //lembrar de comentar antes de enviar
    fclose(fp);

    return 0;
}

void firstOperation(char* arquivoEntrada, char* arquivoSaida) {
    FILE *fp = fopen(arquivoEntrada, "r");
    FILE *out = fopen(arquivoSaida, "wb");

    char line[1024];
    int c = 0;


    Registro *r = initRegister();
    RegistroHeader *rh = initRegisterHeader();


    while (fgets(line, 1024, fp)) {
        if (c == 0) {
            c++;
            continue;
        }

        char *tmp = strdup(line);

        char idNascimento[4] = {0};
        char idadeMae[4] = {0};
        r->cidadeBebe = (char*) malloc (128*sizeof(char));
        r->cidadeMae = (char*) malloc (128*sizeof(char));

        int i = 0;
        int j = 0;

        //sscanf(tmp, "%128[^,],%128[^,],%d[^,],%d[^,],%10[^,],%d[^,],%2[^,],%2[^\n]", r->cidadeMae, r->cidadeBebe, &r->idNascimento, &r->idadeMae, r->dataNascimento, &r->sexoBebe, r->estadoMae, r->estadoBebe);

        for(;;i++) {
            if(tmp[i]==','){
                i++;
                break;
            }
            r->cidadeMae[i] = tmp[i];
        } r->cidadeMae[i] = '\0';
        r->cidadeMae_size = strlen(r->cidadeMae);

        for(;;i++,j++) {
                if(tmp[i]==','){
                    i++; j++;
                    break;
                }
            r->cidadeBebe[j] = tmp[i];
        } r->cidadeBebe[j] = '\0';
        r->cidadeBebe_size = strlen(r->cidadeBebe);
        for(j=0;;j++,i++) {
            if(tmp[i]==','){
                i++;
                break;
            }
            idNascimento[j] = tmp[i];
        }
        r->idNascimento = strtol(idNascimento, NULL, 10);
        for(j=0;;j++,i++) {
            if(tmp[i]==','){
                i++;
                break;
            }
            idadeMae[j] = tmp[i];
        }
        r->idadeMae = strtol(idadeMae, NULL, 10);
        for(j=0;;j++,i++) {
            if(tmp[i]==','){
                i++; j++;
                break;
            }
            r->dataNascimento[j]=tmp[i];
        } r->dataNascimento[j]='\0';
        char aux = tmp[i++];
        if(aux!=','){
            r->sexoBebe = aux - '0';
        } else
            r->sexoBebe = 0;
        i++;
        for(j=0;;j++,i++) {
            if(tmp[i]==','){
                i++; if(j!=0) j++;
                break;
            }

            r->estadoMae[j]=tmp[i];
        } r->estadoMae[j]='\0';
        for(j=0;;j++,i++) {
            if(tmp[i]==',' || tmp[i]=='\n'){
                i++; j++;
                break;
            }
            r->estadoBebe[j]=tmp[i];
        } r->estadoBebe[j]='\0';


        printRegister(r);
        puts(tmp);

        // Add a new register to the binary file
        addRegister(out, r, rh);

        free(tmp);
        free(r->cidadeBebe);
        free(r->cidadeMae);
        c++;
    }

    printf("Leu todo o arquivo: %d\n", c == 1501);

    freeRegister(&r);
    free(rh);

    fclose(fp);
    fclose(out);

    binarioNaTela("out.bin");
}

void Funcionalidade2(char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "rb");

    //se houver erro na abertura do arquivo
    if (fp == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    //se nao houverem registros no arquivo
    if (feof(fp)) {
        printf("Registro inexistente.");
        fclose(fp);
        return;
    }

    int RRN = 0;
    RegistroHeader *rh = readRegisterHeader(fp);
    Registro *r;
    while (RRN != rh->RRNproxRegistro) {
        RRN++;
        r = readRegister(fp, RRN - 1);


        if (r->sexoBebe == 0) {
            printf("Nasceu em %s/%s, em %s, um bebe de sexo IGNORADO.\n",
                   r->cidadeBebe,
                   r->estadoBebe,
                   r->dataNascimento
            );
        } else if (r->sexoBebe == 1) {
            printf("Nasceu em %s/%s, em %s, um bebe de sexo MASCULINO.\n",
                   r->cidadeBebe,
                   r->estadoBebe,
                   r->dataNascimento
            );
        } else if (r->sexoBebe == 2) {
            printf("Nasceu em %s/%s, em %s, um bebe de sexo FEMININO.\n",
                   r->cidadeBebe,
                   r->estadoBebe,
                   r->dataNascimento
            );
        } else {
            printf("leu errado (%d)\n", r->sexoBebe);
        }

        freeRegister(&r);
    }

    free(rh);
    fclose(fp);
}


void printOptions() {
    printf("Escolha a opera��o:\n");
    printf("[1] Opera��o 1.\n");
    printf("[2] Opera��o 2.\n");
    printf("[3] Sair.\n");
}

