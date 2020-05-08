#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _produto {
    char nome[10];
    char marca[10];
    char codigoBarras[13];
    float valor;
} PRODUTO;

void registerNewProduct(FILE *fp);

void printAllProducts(FILE *fp);

void printOperations();

/// 1: Usuário preenche os dados de NOME, MARCA EAN13 e VALOR do produto e é salvo em um arquivo.
/// 2: Usuário lê todos os dados salvos no arquivo.
/// 3: Sai do programa.
int main() {

    FILE *fp = fopen("db.txt", "a+");
    int operation = 0;

    while (1) {
        printOperations();
        scanf("%d", &operation);

        if (operation == 1) {
            registerNewProduct(fp);
        } else if (operation == 2) {
            printAllProducts(fp);
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

void registerNewProduct(FILE *fp) {
    PRODUTO produto;

    // Vai pro final do arquivo.
    fseek(fp, 0, SEEK_END);

    printf("\nNOME (máx 10):\n");
    scanf(" %10s", produto.nome);

    printf("MARCA (máx 10):\n");
    scanf(" %10s", produto.marca);

    printf("CÓDIGO DE BARRAS (máx 13):\n");
    scanf(" %13s", produto.codigoBarras);

    printf("VALOR:\n");
    scanf("%f", &produto.valor);

    fwrite(&produto.nome, sizeof(char), 10, fp);
    fwrite(&produto.marca, sizeof(char), 10, fp);
    fwrite(&produto.codigoBarras, sizeof(char), 13, fp);
    fwrite(&produto.valor, sizeof(float), 1, fp);

    printf("Produto salvo.\n\n");
}

void printAllProducts(FILE *fp) {
    PRODUTO produto;
    int counter = 0;

    // Vai pro começo do arquivo
    fseek(fp, 0, SEEK_SET);

    // Le todo o arquvio de produto em produto
    while (1) {

        fread(&produto.nome, sizeof(char), 10, fp);
        fread(&produto.marca, sizeof(char), 10, fp);
        fread(&produto.codigoBarras, sizeof(char), 13, fp);
        fread(&produto.valor, sizeof(float), 1, fp);

        // Caso tenha dado algum erro lendo esses dados, cgeamos ao fim do arquivo.
        if (feof(fp)) {
            break;
        }

        counter++;
        printf("\nPRODUTO %d:\n", counter);
        printf("NOME: %s\n", produto.nome);
        printf("MARCA: %s\n", produto.marca);
        printf("CÓDIGO DE BARRAS: %s\n", produto.codigoBarras);
        printf("VALOR: %.2f\n", produto.valor);
        printf("-----\n");
    }

    printf("Fim do arquivo (foram lidos %d produtos).\n\n", counter);
}

void printOperations() {
    printf("Escolha a operação:\n");
    printf("[1] Salvar novo produto.\n");
    printf("[2] Ler todos os produtos.\n");
    printf("[3] Sair.\n");
}