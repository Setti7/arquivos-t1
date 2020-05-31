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

void Funcionalidade3(char *nomeArquivo, int m);

void Funcionalidade4(char *nomeArquivo, int rrn);

void Funcionalidade5(char *nomeArquivo, int n);

void Funcionalidade7(char *nomeArquivo, int n);

int main() {

    int operation = 0;
    char arquivoEntrada[100], arquivoSaida[100];

    scanf("%d", &operation);

    switch (operation) { //Executa as funcionalidades conforme definidas.
        case 1 :
            scanf("%s %s", arquivoEntrada, arquivoSaida);
            firstOperation(arquivoEntrada, arquivoSaida);
            break;
        case 2 :
            scanf("%s", arquivoEntrada);
            Funcionalidade2(arquivoEntrada);
        case 3 :
            scanf("%s %d", arquivoEntrada, &operation);
            Funcionalidade3(arquivoEntrada, operation);
            break;
        case 4 :
            scanf("%s %d", arquivoEntrada,
                  &operation); //como a variavel de operacao ja foi usada, podemos reaproveitar ela pra alocar o rrn do registro desejado.
            Funcionalidade4(arquivoEntrada, operation);
            break;
        case 5 :
            scanf("%s %d", arquivoEntrada, &operation);
            Funcionalidade5(arquivoEntrada, operation);
            break;
        case 6 :
            break;
        case 7 : {
            int n = 0;
            scanf("%s %d", arquivoEntrada, &n);
            Funcionalidade7(arquivoEntrada, n);
            break;
        }
    }

    return 0;
}

int ComparaCamposNaoNulos(Registro *r, Registro *rBusca) {
    int igual = 0;
    if(rBusca->cidadeBebe!=NULL){
        if (strcmp(r->cidadeBebe,rBusca->cidadeBebe)==0) {
            igual = 1;
        }
        else
            return 0;
    }
    if(rBusca->cidadeMae!=NULL){
        if (strcmp(r->cidadeMae,rBusca->cidadeMae)==0) {
            igual = 1;
        }
        else
            return 0;
    }
    if(strcmp(rBusca->estadoBebe,"\0")!=0){
        if (strcmp(r->estadoBebe,rBusca->estadoBebe)==0) {
            igual = 1;
        }
        else
            return 0;
    }
    if(strcmp(rBusca->estadoMae,"\0")!=0){
        if (strcmp(r->estadoMae,rBusca->estadoMae)==0) {
            igual = 1;
        }
        else
            return 0;
    }
    if(strcmp(rBusca->dataNascimento,"\0")!=0){
        if (strcmp(r->dataNascimento,rBusca->dataNascimento)==0) {
            igual = 1;
        }
        else
            return 0;
    }
    if(rBusca->idNascimento!=0){
        if (r->idNascimento==rBusca->idNascimento) {
            igual = 1;
        }
        else
            return 0;
    }
    if(rBusca->sexoBebe!='\0'){
        if (r->sexoBebe==rBusca->sexoBebe) {
            igual = 1;
        }
        else
            return 0;
    }
    if(rBusca->idadeMae!=0){
        if (r->idadeMae==rBusca->idadeMae) {
            igual = 1;
        }
        else
            return 0;
    }
    return igual;
}

Registro *MontarCampos(int numberOfFieds) {
    Registro *r = initRegister();
    char nomeCampo[128];
    char valorCampo[128];
    for (int m = 0; m < numberOfFieds; m++) {
        scanf("%s", nomeCampo);
        scan_quote_string(valorCampo);

        if (strcmp(nomeCampo, "cidadeMae") == 0) {
            //se o campo for cidadeMae

            if (valorCampo[0] == '\0') {
                r->cidadeMae_size = 0;
            } else {
                r->cidadeMae_size = strlen(valorCampo);
                r->cidadeMae = malloc(sizeof(char) * r->cidadeMae_size);
                strcpy(r->cidadeMae, valorCampo);
            }
        } else if (strcmp(nomeCampo, "cidadeBebe") == 0) {
            // se o campo for cidadeBebe

            if (valorCampo[0] == '\0') {
                r->cidadeBebe_size = 0;
            } else {
                r->cidadeBebe_size = strlen(valorCampo);
                r->cidadeBebe = malloc(sizeof(char) * r->cidadeBebe_size);
                strcpy(r->cidadeBebe, valorCampo);
            }

        } else if (strcmp(nomeCampo, "idNascimento") == 0) {
            // se o campo for idNascimento, apenas atualize seu valor (não precisa tratar)
            r->idNascimento = strtol(valorCampo, NULL, 10);
        } else if (strcmp(nomeCampo, "idadeMae") == 0) {
            // idade mae precisa tratar valores nulos com -1
            if (valorCampo[0] == '\0') {
                r->idadeMae = -1;
            } else {
                r->idadeMae = strtol(valorCampo, NULL, 10);
            }
        } else if (strcmp(nomeCampo, "dataNascimento") == 0) {
            // dataNascimento precisa tratar valores nulos

            if (valorCampo[0] == '\0') {
                r->dataNascimento[0] = '\0';
            } else {
                strcpy(r->dataNascimento, valorCampo);
            }
        } else if (strcmp(nomeCampo, "sexoBebe") == 0) {
            // sexoBebe precisa tratar valores nulos

            if (valorCampo[0] == '\0') {
                r->sexoBebe = '0';
            } else {
                r->sexoBebe = valorCampo[0];
            }
        } else if (strcmp(nomeCampo, "estadoMae") == 0) {
            // estadoMae precisa tratar valores nulos

            if (valorCampo[0] == '\0') {
                r->estadoMae[0] = '\0';
            } else {
                strcpy(r->estadoMae, valorCampo);
            }
        } else if (strcmp(nomeCampo, "estadoBebe") == 0) {
            // estadoBebe precisa tratar valores nulos

            if (valorCampo[0] == '\0') {
                r->estadoBebe[0] = '\0';
            } else {
                strcpy(r->estadoBebe, valorCampo);
            }
        }

    }

    return r;
}

void PrintR(Registro *r) {
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
}

void firstOperation(char *arquivoEntrada, char *arquivoSaida) {
    FILE *fp = fopen(arquivoEntrada, "r");
    FILE *out = fopen(arquivoSaida, "wb");

    if (fp == NULL || out == NULL) {
        printf("Falha no carregamento do arquivo.");
        return;
    }

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
        fclose(fp);
        return;
    }

    //se nao houverem registros no arquivo
    if (feof(fp)) {
        printf("Registro Inexistente.");
        free(rh);
        fclose(fp);
        return;
    }

    if (rh->RRNproxRegistro == 0) {
        printf("Registro Inexistente.");
        free(rh);
        fclose(fp);
        return;
    }

    while (RRN != rh->RRNproxRegistro) {
        RRN++;

        Registro *r = readRegister(fp, RRN - 1);

        if(r==NULL)
            continue;
        //imprime o registro
        PrintR(r);

        if (r->cidadeMae_size >= 0) free(r->cidadeMae);
        if (r->cidadeBebe_size >= 0) free(r->cidadeBebe);
        freeRegister(&r);
    }

    free(rh);
    fclose(fp);
}

void Funcionalidade3(char *nomeArquivo, int m) {
    FILE *fp = fopen(nomeArquivo, "rb");
    int imprimiu =0;
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
        fclose(fp);
        return;
    }

    //se nao houverem registros no arquivo
    if (feof(fp)) {
        printf("Registro Inexistente.");
        free(rh);
        fclose(fp);
        return;
    }
    Registro *rBusca = MontarCampos(m);

    if (rh->RRNproxRegistro == 0) {
        printf("Registro Inexistente.");
        free(rh);
        fclose(fp);
        return;
    }

    while (RRN != rh->RRNproxRegistro) {
        RRN++;

        Registro *r = readRegister(fp, RRN - 1);

        if(r==NULL)
            continue;

        if(ComparaCamposNaoNulos(r,rBusca)==0)
            continue;
        //imprime o registro
        PrintR(r);
        imprimiu++;
        if (r->cidadeMae_size >= 0) free(r->cidadeMae);
        if (r->cidadeBebe_size >= 0) free(r->cidadeBebe);
        freeRegister(&r);
    }
    freeRegister(&rBusca);
    if(imprimiu==0){
        printf("Registro Inexistente.");
    }
}

void Funcionalidade4(char *nomeArquivo, int rrn) {
    FILE *fp = fopen(nomeArquivo, "rb");

    //se houver erro na abertura do arquivo
    if (fp == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    RegistroHeader *rh = readRegisterHeader(fp);
    if (rh->status != '1') {
        printf("Falha no processamento do arquivo.");
        free(rh);
        fclose(fp);
        return;
    }

    //se nao houverem registros no arquivo
    if (feof(fp)) {
        printf("Registro Inexistente.");
        fclose(fp);
        return;
    }

    //busca o registro de rrn desejado
    Registro *r = readRegister(fp, rrn);

    if(r==NULL) { //se o registro foi removido;
        printf("Registro Inexistente.");
        return;
    }

    //imprime o registro
    PrintR(r);

    //libera a memoria e fecha o arquivo
    if (r->cidadeMae_size >= 0) free(r->cidadeMae);
    if (r->cidadeBebe_size >= 0) free(r->cidadeBebe);
    freeRegister(&r);
    free(rh);
    fclose(fp);
}

void Funcionalidade5(char *nomeArquivo, int n){
    FILE *fp = fopen(nomeArquivo, "r+b");
     //se houver erro na abertura do arquivo
    if (fp == NULL) {
        printf("Falha no processamento do arquivo.");
        return;
    }

    RegistroHeader *rh = readRegisterHeader(fp);
    int RRN = 0;
    if (rh->status != '1') {
        printf("Falha no processamento do arquivo.");
        free(rh);
        fclose(fp);
        return;
    }
    for(int i = 0; i < n; i++){
        int m = 0;
        scanf("%d", &m);
        Registro *rBusca = MontarCampos(m);
        while (RRN != rh->RRNproxRegistro) {
            RRN++;

            Registro *r = readRegister(fp, RRN - 1);

            if(r==NULL)
                continue;

            if(ComparaCamposNaoNulos(r,rBusca)==0)
                continue;
            //remove registro
            deleteRegister(fp, RRN-1, rh);

            if (r->cidadeMae_size >= 0) free(r->cidadeMae);
            if (r->cidadeBebe_size >= 0) free(r->cidadeBebe);
            freeRegister(&r);
        }
        freeRegister(&rBusca);
    }

    free(rh);
    fclose(fp);

    binarioNaTela(nomeArquivo);
}

void Funcionalidade7(char *nomeArquivo, int n) {
    FILE *fp = fopen(nomeArquivo, "r+b");

    for (int i = 0; i < n; i++) {
        int rrn;
        int numberOfFieds;
        char nomeCampo[128];
        char valorCampo[128];

        scanf("%d %d", &rrn, &numberOfFieds);

        // le o registro no dado rrn, caso volte nulo ele foi removido.
        Registro *r = readRegister(fp, rrn);
        if (r == NULL) continue;

        // DBEUG
        // printRegister(r);

        for (int m = 0; m < numberOfFieds; m++) {
            scanf("%s", nomeCampo);
            scan_quote_string(valorCampo);

            if (strcmp(nomeCampo, "cidadeMae") == 0) {
                //se o campo for cidadeMae

                if (valorCampo[0] == '\0') {
                    r->cidadeMae_size = 0;
                } else {
                    r->cidadeMae_size = strlen(valorCampo);
                    r->cidadeMae = malloc(sizeof(char) * r->cidadeMae_size);
                    strcpy(r->cidadeMae, valorCampo);
                }
            } else if (strcmp(nomeCampo, "cidadeBebe") == 0) {
                // se o campo for cidadeBebe

                if (valorCampo[0] == '\0') {
                    r->cidadeBebe_size = 0;
                } else {
                    r->cidadeBebe_size = strlen(valorCampo);
                    r->cidadeBebe = malloc(sizeof(char) * r->cidadeBebe_size);
                    strcpy(r->cidadeBebe, valorCampo);
                }

            } else if (strcmp(nomeCampo, "idNascimento") == 0) {
                // se o campo for idNascimento, apenas atualize seu valor (não precisa tratar)
                r->idNascimento = strtol(valorCampo, NULL, 10);
            } else if (strcmp(nomeCampo, "idadeMae") == 0) {
                // idade mae precisa tratar valores nulos com -1
                if (valorCampo[0] == '\0') {
                    r->idadeMae = -1;
                } else {
                    r->idadeMae = strtol(valorCampo, NULL, 10);
                }
            } else if (strcmp(nomeCampo, "dataNascimento") == 0) {
                // dataNascimento precisa tratar valores nulos

                if (valorCampo[0] == '\0') {
                    r->dataNascimento[0] = '\0';
                } else {
                    strcpy(r->dataNascimento, valorCampo);
                }
            } else if (strcmp(nomeCampo, "sexoBebe") == 0) {
                // sexoBebe precisa tratar valores nulos

                if (valorCampo[0] == '\0') {
                    r->sexoBebe = '0';
                } else {
                    r->sexoBebe = valorCampo[0];
                }
            } else if (strcmp(nomeCampo, "estadoMae") == 0) {
                // estadoMae precisa tratar valores nulos

                if (valorCampo[0] == '\0') {
                    r->estadoMae[0] = '\0';
                } else {
                    strcpy(r->estadoMae, valorCampo);
                }
            } else if (strcmp(nomeCampo, "estadoBebe") == 0) {
                // estadoBebe precisa tratar valores nulos

                if (valorCampo[0] == '\0') {
                    r->estadoBebe[0] = '\0';
                } else {
                    strcpy(r->estadoBebe, valorCampo);
                }
            }

        }

//        printRegister(r);
        updateRegister(fp, rrn, r);

        if (r->cidadeMae_size >= 0) free(r->cidadeMae);
        if (r->cidadeBebe_size >= 0) free(r->cidadeBebe);
        freeRegister(&r);
    }

    fclose(fp);

    binarioNaTela(nomeArquivo);
}
