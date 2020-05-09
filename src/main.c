/*
 * Organização de Arquivos - T1
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

void firstOperation();

int main() {
    firstOperation();
    return 0;
}

void firstOperation() {
    FILE *fp = fopen("in.csv", "r");
    FILE *out = fopen("out.bin", "wb");

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

        char cidadeMae[128] = {0};
        char cidadeBebe[128] = {0};
        char idNascimento[4] = {0};
        char idadeMae[4] = {0};
        char dataNascimento[11] = {0};
        char sexoBebe[4] = {0};
        char estadoMae[3] = {0};
        char estadoBebe[3] = {0};

        sscanf(tmp, "%128[^,],%128[^,],%4[^,],%4[^,],%11[^,],%4[^,],%3[^,],%3[^\n]", cidadeMae, cidadeBebe,
               idNascimento,
               idadeMae, dataNascimento, sexoBebe, estadoMae, estadoBebe);

        r->cidadeMae = cidadeMae;
        r->cidadeMae_size = strlen(cidadeMae);
        r->cidadeBebe_size = strlen(cidadeBebe);
        r->cidadeBebe = cidadeBebe;
        r->idNascimento = strtol(idNascimento, NULL, 10);;
        r->idadeMae = strtol(idadeMae, NULL, 10);;
        strcpy(r->dataNascimento, dataNascimento);
        r->sexoBebe = strtol(sexoBebe, NULL, 10);
        strcpy(r->estadoMae, estadoMae);
        strcpy(r->estadoBebe, estadoBebe);

        printRegister(r);
        puts(tmp);

        // Add a new register to the binary file
        addRegister(out, r, rh);

        free(tmp);
        c++;
    }

    printf("Leu todo o arquivo: %d\n", c == 1501);

    free(r);
    free(rh);

    fclose(fp);
    fclose(out);

    binarioNaTela("out.bin");
}
