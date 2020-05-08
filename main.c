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

    FILE *fp = fopen("out.bin", "wb"); //wb para escrever em binario
    int operation = 0;

    while (1) {
        printOptions(); //lembrar de comentar antes de enviar
        scanf("%d", &operation);

        if (operation == 1) {
        } else if (operation == 2) {
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

void Funcionalidade2(char* nomeArquivo){
    FILE *arquivoBinario = fopen(nomeArquivo, "rb");
    Registro reg;
    //se houver erro na abertura do arquivo
    if(arquivoBinario==NULL) { 
        printf("Falha no processamento do arquivo.");
        return;
    }
    //se nao houverem registros no arquivo
    if(feof(arquivoBinario)) { 
        printf("Registro inexistente.");
        fclose(arquivoBinario);
        return;
    }
    fseek(arquivoBinario, 128, SEEK_SET); //pula o cabecalho
    //enquanto ainda houver registros a serem lidos
    while (!feof(arquivoBinario)) { 
        //le 1 registro
        fread(&reg.cidadeMae_size, sizeof(int), 1, arquivoBinario); //pega o tamanho do campo cidadeMae
        fread(&reg.cidadeBebe_size, sizeof(int), 1, arquivoBinario); //pega o tamanho do campo cidadeBebe
        fseek(arquivoBinario, reg.cidadeMae_size, SEEK_CUR); //pula para o campo cidadeBebe
        reg.cidadeBebe = (char*) malloc (reg.cidadeBebe_size*sizeof(char)); //aloca o espaco da cidadeBebe
        fread(reg.cidadeBebe, reg.cidadeBebe_size*sizeof(char), 1, arquivoBinario);
        fseek(arquivoBinario, 105-(8+reg.cidadeMae_size+reg.cidadeBebe_size)+8, SEEK_CUR); //pula pro campo dataNascimento
        fread(&reg.dataNascimento, 10*sizeof(char), 1, arquivoBinario);
        fread(&reg.sexoBebe, sizeof(int), 1, arquivoBinario);
        fseek(arquivoBinario, 2, SEEK_CUR); //pula para o campo estadoBebe
        fread(&reg.estadoBebe, 2*sizeof(char), 1, arquivoBinario);
        if(reg.sexoBebe==0)
            printf("Nasceu em %s/%s, em %s, um bebe de sexo IGNORADO.\n", reg.cidadeBebe, reg.estadoBebe, reg.dataNascimento);
        else if(registro.sexoBebe==1)
            printf("Nasceu em %s/%s, em %s, um bebe de sexo MASCULINO.\n", reg.cidadeBebe, reg.estadoBebe, reg.dataNascimento);
        else if(registro.sexoBebe==2)
            printf("Nasceu em %s/%s, em %s, um bebe de sexo FEMININO.\n", reg.cidadeBebe, reg.estadoBebe, reg.dataNascimento);
        free(reg.cidadeBebe);
    }
    fclose(arquivoBinario);
}

void printOptions() {
    printf("Escolha a operação:\n");
    printf("[1] Operação 1.\n");
    printf("[2] Operação 2.\n");
    printf("[3] Sair.\n");
}

