#include <stdio.h>

typedef struct registroHeader {
    char status;
    int RRNproxRegistro;
    int numeroRegistrosInseridos;
    int numeroRegistrosRemovidos;
    int numeroRegistrosAtualizados;
    char lixo[111];
} RegistroHeader;


typedef struct registro {
    int cidadeMae_size;
    int cidadeBebe_size;
    char *cidadeMae; // sem /0
    char *cidadeBebe; // sem /0
    int idNascimento;
    int idadeMae;
    char dataNascimento[11]; // sem /0, se for nulo tem que estar /0$$$$$$$$$
    char sexoBebe;
    char estadoMae[3];
    char estadoBebe[3];
} Registro;

void printOptions();

int main() {

    FILE *fp = fopen("out.bin", "w");
    int operation = 0;

    while (1) {
        printOptions();
        scanf("%d", &operation);

        if (operation == 1) {
        } else if (operation == 2) {
        } else if (operation == 3) {
            break;
        } else {
            printf("Operação inválida.\n");
        }
    }

    printf("Saindo...");
    fclose(fp);

    return 0;
}

void printOptions() {
    printf("Escolha a operação:\n");
    printf("[1] Operação 1.\n");
    printf("[2] Operação 2.\n");
    printf("[3] Sair.\n");
}

