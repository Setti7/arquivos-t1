/*
 * Organizaçãoo de Arquivos - T1
 *
 * Feito por:
 *  André Niero Setti - 10883901
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

        Registro *r = initRegister();
        r->cidadeBebe = calloc(128, sizeof(char));
        r->cidadeMae = calloc(128, sizeof(char));

        int i = 0;
        int j = 0;

        // Começa parse maluco do csv
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

//        DEBUG
//        printRegister(r);
//        puts(tmp);

        // Add a new register to the binary file
        addRegister(out, r, rh);

        free(tmp);

        free(r->cidadeMae);
        free(r->cidadeBebe);
        freeRegister(&r);

        c++;
    }

    free(rh);

    fclose(fp);
    fclose(out);

    binarioNaTela(arquivoSaida);
}

void Funcionalidade2(char *nomeArquivo) {
    FILE *fp = fopen(nomeArquivo, "rb");

    //se houver erro na abertura do arquivo
    if (fp == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }
    int RRN = 0;
    RegistroHeader *rh = readRegisterHeader(fp);
    if (rh->status != '1') {
        printf("Falha no processamento do arquivo.");
        free(rh);
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

        Registro *r = readRegister(fp, RRN - 1);

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
            // DEBUG
            printRegister(r);
            printf("Nasceu em %s/%s, em %s, um bebê de sexo INDEFINIDO (%c).\n",
                   r->cidadeBebe_size > 0 ? r->cidadeBebe : "-",
                   strlen(r->estadoBebe) == 0 ? "-" : r->estadoBebe,
                   strlen(r->dataNascimento) > 0 ? r->dataNascimento : "-",
                   r->sexoBebe
            );
        }

        if (r->cidadeMae_size > 0) free(r->cidadeMae);
        if (r->cidadeBebe_size > 0) free(r->cidadeBebe);
        freeRegister(&r);
    }

    free(rh);
    fclose(fp);
}
