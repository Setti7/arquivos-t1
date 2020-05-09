#include <stdio.h>
#include <stdlib.h>

int main() {

    FILE *fp = fopen("out.bin", "wb"); //wb para escrever em binario
    int operation = 0;

    while (1) {
        printOptions(); //lembrar de comentar antes de enviar
        scanf("%d", &operation);

        if (operation == 1) {
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

