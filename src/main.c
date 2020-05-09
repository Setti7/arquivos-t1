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

void printOptions();

void Funcionalidade2();

int main() {

    FILE *fp = fopen("out.bin", "wb"); //wb para escrever em binario
    int operation = 0;

    while (1) {
        printOptions(); //lembrar de comentar antes de enviar
        scanf("%d", &operation);

        if (operation == 1) {
            firstOperation();
        } else if (operation == 2) {
            char nomeArquivo[100];
            scanf("%s", nomeArquivo);
            Funcionalidade2(nomeArquivo);
        } else if (operation == 3) {
            break;
        } else {
            printf("Operação inválida.\n"); //lembrar de comentar antes de enviar
        }
    }

    printf("Saindo..."); //lembrar de comentar antes de enviar
    fclose(fp);

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

void Funcionalidade2(char *nomeArquivo) {
    FILE *arquivoBinario = fopen(nomeArquivo, "rb");
    Registro *reg = initRegister();

    //se houver erro na abertura do arquivo
    if (arquivoBinario == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    //se nao houverem registros no arquivo
    if (feof(arquivoBinario)) {
        printf("Registro inexistente.");
        fclose(arquivoBinario);
        return;
    }

    fseek(arquivoBinario, 128, SEEK_SET); //pula o cabecalho

    //enquanto ainda houver registros a serem lidos
    while (!feof(arquivoBinario)) {
        //le 1 registro
        fread(&reg->cidadeMae_size, sizeof(int), 1, arquivoBinario); //pega o tamanho do campo cidadeMae
        fread(&reg->cidadeBebe_size, sizeof(int), 1, arquivoBinario); //pega o tamanho do campo cidadeBebe
        fseek(arquivoBinario, reg->cidadeMae_size, SEEK_CUR); //pula para o campo cidadeBebe
        reg->cidadeBebe = (char *) malloc(reg->cidadeBebe_size * sizeof(char)); //aloca o espaco da cidadeBebe
        fread(reg->cidadeBebe, reg->cidadeBebe_size * sizeof(char), 1, arquivoBinario);
        fseek(arquivoBinario, 105 - (8 + reg->cidadeMae_size + reg->cidadeBebe_size) + 8,
              SEEK_CUR); //pula pro campo dataNascimento
        fread(&reg->dataNascimento, 10 * sizeof(char), 1, arquivoBinario);
        fread(&reg->sexoBebe, sizeof(int), 1, arquivoBinario);
        fseek(arquivoBinario, 2, SEEK_CUR); //pula para o campo estadoBebe
        fread(&reg->estadoBebe, 2 * sizeof(char), 1, arquivoBinario);
        if (reg->sexoBebe == 0)
            printf("Nasceu em %s/%s, em %s, um bebe de sexo IGNORADO.\n", reg->cidadeBebe, reg->estadoBebe,
                   reg->dataNascimento);
        else if (reg->sexoBebe == 1)
            printf("Nasceu em %s/%s, em %s, um bebe de sexo MASCULINO.\n", reg->cidadeBebe, reg->estadoBebe,
                   reg->dataNascimento);
        else if (reg->sexoBebe == 2)
            printf("Nasceu em %s/%s, em %s, um bebe de sexo FEMININO.\n", reg->cidadeBebe, reg->estadoBebe,
                   reg->dataNascimento);
        free(reg->cidadeBebe);
    }
    fclose(arquivoBinario);
}


void printOptions() {
    printf("Escolha a operação:\n");
    printf("[1] Operação 1.\n");
    printf("[2] Operação 2.\n");
    printf("[3] Sair.\n");
}

