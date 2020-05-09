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

#define FIRST_FIELD -1
#define NORMAL_FIELD 0
#define LAST_FIELD 1

void printOptions();

void firstOperation();

int main() {

    int operation = 0;

    while (1) {
        printOptions();
        scanf("%d", &operation);

        if (operation == 1) {
            firstOperation();
        } else if (operation == 3) {
            break;
        } else {
            printf("Operação inválida.\n");
        }
    }

    printf("Saindo...");

    return 0;
}

char *getNextField(char *line, int field) {

    if (field == FIRST_FIELD) {
        return strtok(line, ",");
    } else if (field == NORMAL_FIELD) {
        return strtok(NULL, ",");
    } else if (field == LAST_FIELD) {
        return strtok(line, "\n");
    }
    return NULL;
}


void firstOperation() {
    char inputFileName[10];

//    scanf("%s", inputFileName);
//    printf("%s\n", inputFileName); // DEBUG
//
//    // reading csv file
//    FILE *fp = fopen(inputFileName, "r");
    FILE *fp = fopen("in.csv", "r");
    FILE *out = fopen("out.bin", "wb");

    char line[1024];
    int c = 0;
    char *value;

    Registro *r = malloc(sizeof(Registro));
    RegistroHeader *rh = calloc(1, sizeof(RegistroHeader));

    while (fgets(line, 1024, fp)) {
        if (c == 0) {
            c++;
            writeHeaderRegister(out, rh);
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

        sscanf(tmp, "%128[^,],%128[^,],%4[^,],%4[^,],%11[^,],%4[^,],%3[^,],%3[^\n]", cidadeMae, cidadeBebe, idNascimento,
               idadeMae, dataNascimento, sexoBebe, estadoMae, estadoBebe);



        printf("");

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


//        sscanf(tmp, "%[^,]%[^,]%[^,]%[^,]%[^,]%[^,]%[^,]%[^,]", r->cidadeMae, r->cidadeBebe, idNascimento,
//               idadeMae, r->dataNascimento, sexoBebe, r->estadoMae, r->estadoBebe);

//        fscanf(fp, "%s,%s,%s,%s,%s,%s,%s,%s\n", r->cidadeMae, r->cidadeBebe, idNascimento, idadeMae,
//               r->dataNascimento, sexoBebe, r->estadoMae, r->estadoBebe);

//        r->idNascimento = strtol(idNascimento, NULL, 10);
//        r->idadeMae = strtol(idadeMae, NULL, 10);
//        r->sexoBebe = strtol(sexoBebe, NULL, 10);

        // Reading values from CSV
//        value = strtok(line, ",");
//        r->cidadeMae_size = strlen(value);
//        r->cidadeMae = value;
//
//        value = getNextField(NULL, NORMAL_FIELD);
//        r->cidadeBebe_size = strlen(value);
//        r->cidadeBebe = value;
//
//        value = getNextField(NULL, NORMAL_FIELD);
//        r->idNascimento = strtol(value, NULL, 10);
//
//        value = getNextField(NULL, NORMAL_FIELD);
//        r->idadeMae = strtol(value, NULL, 10);
//
//        value = getNextField(NULL, NORMAL_FIELD);
//        strcpy(r->dataNascimento, value == NULL ? "\0" : value);
//
//        value = getNextField(NULL, NORMAL_FIELD);
//        r->sexoBebe = strtol(value, NULL, 10);
//
//        value = getNextField(NULL, NORMAL_FIELD);
//        strcpy(r->estadoMae, value == NULL ? "\0" : value);
//
//        value = getNextField(NULL, LAST_FIELD);
//        strcpy(r->estadoBebe, value == NULL ? "\0" : value);

        // Add a new register to the binary file
        addRegister(out, r, rh);

        printf("\n\n");

        free(tmp);
        c++;
//        if (c == 10) {
//            break;
//        }
    }

    fclose(fp);
    fclose(out);

    binarioNaTela("out.bin");
}

void printOptions() {
    printf("Escolha a operação:\n");
    printf("[1] Gravar registro a partir de .csv\n");
    printf("[2] Ler dados do .bin\n");
    printf("[3] Sair.\n");
}

