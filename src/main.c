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

void firstOperation(char *arquivoEntrada, char *arquivoSaida);

void Funcionalidade2(char *nomeArquivo);

int main() {

    int operation = 0;

    scanf("%d", &operation);

    if (operation == 1) {
        char arquivoEntrada[100], arquivoSaida[100];
        scanf("%s %s", arquivoEntrada, arquivoSaida);
        firstOperation(arquivoEntrada, arquivoSaida);
    } else if (operation == 2) {
        char nomeArquivo[100];
        scanf("%s", nomeArquivo);
        Funcionalidade2(nomeArquivo);
    }

    return 0;
}

void firstOperation(char *arquivoEntrada, char *arquivoSaida) {
    FILE *fp = fopen(arquivoEntrada, "r");
    FILE *out = fopen(arquivoSaida, "wb");

    char line[1024];
    int c = 0;


    Registro *r = initRegister();
    RegistroHeader *rh = initRegisterHeader();
    writeHeaderRegister(out, rh);


    while (fgets(line, 1024, fp)) {
        if (c == 0) {
            c++;
            continue;
        }

        char *tmp = strdup(line);

        char idNascimento[4] = {0};
        char idadeMae[4] = {0};
        r->cidadeBebe = (char *) malloc(30 * sizeof(char));
        r->cidadeMae = (char *) malloc(30 * sizeof(char));

        int i = 0;
        int j = 0;

        //sscanf(tmp, "%128[^,],%128[^,],%d[^,],%d[^,],%10[^,],%d[^,],%2[^,],%2[^\n]", r->cidadeMae, r->cidadeBebe, &r->idNascimento, &r->idadeMae, r->dataNascimento, &r->sexoBebe, r->estadoMae, r->estadoBebe);

        for (;; i++) {
            if (tmp[i] == ',') {
                i++;
                break;
            }
            r->cidadeMae[i] = tmp[i];
        }
        r->cidadeMae[i - 1] = '\0';
        r->cidadeMae_size = strlen(r->cidadeMae);

        for (;; i++, j++) {
            if (tmp[i] == ',') {
                i++;
                break;
            }
            r->cidadeBebe[j] = tmp[i];
        }
        r->cidadeBebe[j] = '\0';
        r->cidadeBebe_size = strlen(r->cidadeBebe);
        for (j = 0;; j++, i++) {
            if (tmp[i] == ',') {
                i++;
                break;
            }
            idNascimento[j] = tmp[i];
        }
        r->idNascimento = strtol(idNascimento, NULL, 10);
        for (j = 0;; j++, i++) {
            if (tmp[i] == ',') {
                i++;
                break;
            }
            idadeMae[j] = tmp[i];
        }
        r->idadeMae = strtol(idadeMae, NULL, 10);
        for (j = 0;; j++, i++) {
            if (tmp[i] == ',') {
                i++;
                break;
            }
            r->dataNascimento[j] = tmp[i];
        }
        r->dataNascimento[j] = '\0';
        char aux = tmp[i++];
        if (aux != ',') {
            r->sexoBebe = aux;
        } else {
            r->sexoBebe = '0';
            i--;
        }
        i++;
        for (j = 0;; j++, i++) {
            if (tmp[i] == ',') {
                i++;
                break;
            }
            r->estadoMae[j] = tmp[i];
        }
        r->estadoMae[j] = '\0';
        for (j = 0;; j++, i++) {
            if (tmp[i] == ',' || tmp[i] == '\n') {
                i++;
                break;
            }
            r->estadoBebe[j] = tmp[i];
        }
        r->estadoBebe[j] = '\0';


//        printRegister(r);
//        puts(tmp);

        // Add a new register to the binary file
        addRegister(out, r, rh);

        free(tmp);

        c++;
    }

    writeHeaderRegister(out, rh);
    freeRegister(&r);
    free(rh);

    fclose(fp);
    fclose(out);

    binarioNaTela(arquivoSaida);
}

void FalhaProcessamento() {
    printf("Falha no processamento do arquivo.");
    return;
}

void Funcionalidade2(char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "rb");

    //se houver erro na abertura do arquivo
    if (fp == NULL) {
        FalhaProcessamento();
        return;
    }
    int RRN = 0;
    RegistroHeader *rh = readRegisterHeader(fp);
    Registro *r;
    if (rh->status != '1') {
        FalhaProcessamento();
        return;
    }

    //se nao houverem registros no arquivo
    if (feof(fp)) {
        printf("Registro inexistente.");
        fclose(fp);
        return;
    }

    while (RRN != rh->RRNproxRegistro) {
        RRN++;
        r = readRegister(fp, RRN - 1);


        if (r->sexoBebe == '0') {
            printf("Nasceu em %s/%s, em %s, um bebê de sexo IGNORADO.\n",
                   r->cidadeBebe_size > 0 ? r->cidadeBebe : "-",
                   strlen(r->estadoBebe) == 0 ? "-" : r->estadoBebe,
                   strlen(r->dataNascimento) > 0 ? r->dataNascimento : "-"
            );
        } else if (r->sexoBebe == '1') {
            printf("Nasceu em %s/%s, em %s, um bebê de sexo MASCULINO.\n",
                   r->cidadeBebe_size > 0 ? r->cidadeBebe : "-",
                   strlen(r->estadoBebe) == 0 ? "-" : r->estadoBebe,
                   strlen(r->dataNascimento) > 0 ? r->dataNascimento : "-"
            );
        } else if (r->sexoBebe == '2') {
            printf("Nasceu em %s/%s, em %s, um bebê de sexo FEMININO.\n",
                   r->cidadeBebe_size > 0 ? r->cidadeBebe : "-",
                   strlen(r->estadoBebe) == 0 ? "-" : r->estadoBebe,
                   strlen(r->dataNascimento) > 0 ? r->dataNascimento : "-"
            );
        } else {
            printRegister(r);
            printf("Nasceu em %s/%s, em %s, um bebê de sexo INDEFINIDO (%c).\n",
                   r->cidadeBebe_size > 0 ? r->cidadeBebe : "-",
                   strlen(r->estadoBebe) == 0 ? "-" : r->estadoBebe,
                   strlen(r->dataNascimento) > 0 ? r->dataNascimento : "-",
                   r->sexoBebe
            );
        }

        freeRegister(&r);
    }

    free(rh);
    fclose(fp);
}
