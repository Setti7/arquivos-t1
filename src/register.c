#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "register.h"

#define REGISTER_OPERATION short int
#define REGISTER_OVERWRITE -1
#define REGISTER_UPDATE -2

Registro *initRegister() {
    /*
     * Inicializa um registro. Caso use os campos de tamaho variavél, eles devem ser alocados individualmente (e free'd
     * também).
     * */

    return calloc(1, sizeof(Registro));
}

void setRegisterField(Registro *r, char *fieldName, void *fieldValue) {
    /*
     * Atualiza um campo do registro com o valor dado.
     *
     * cidadeMae: String
     * cidadeBebe: String
     * idNascimento: int
     * idadeMae: int
     * dataNascimento: String
     * sexoBebe: char ou String
     * estadoMae: String
     * estadoBebe: String
     * */

    if (strcmp(fieldName, "cidadeMae") == 0) {
        //se o campo for cidadeMae

        if (strlen(fieldValue) == 0) {
            r->cidadeMae_size = 0;
        } else {
            r->cidadeMae_size = strlen(fieldValue);

            // realloc para ocupar o novo espaço da cidade mae (size + 1, pois tem que haver espaço para o \0)
            // realloc chama malloc se  ainda não foi allocado, então não precisamos tratar.
            r->cidadeMae = realloc(r->cidadeMae, sizeof(char) * r->cidadeMae_size + 1);
            strcpy(r->cidadeMae, fieldValue);
        }
    } else if (strcmp(fieldName, "cidadeBebe") == 0) {
        // se o campo for cidadeBebe

        if (strlen(fieldValue) == 0) {
            r->cidadeBebe_size = 0;
        } else {
            r->cidadeBebe_size = strlen(fieldValue);

            // realloc para ocupar o novo espaço da cidade mae (size + 1, pois tem que haver espaço para o \0)
            // realloc chama malloc se  ainda não foi allocado, então não precisamos tratar.
            r->cidadeBebe = realloc(r->cidadeBebe, sizeof(char) * r->cidadeBebe_size + 1);
            strcpy(r->cidadeBebe, fieldValue);
        }

    } else if (strcmp(fieldName, "idNascimento") == 0) {
        // se o campo for idNascimento, apenas atualize seu valor (não precisa tratar)
        r->idNascimento = *(int *) fieldValue;
    } else if (strcmp(fieldName, "idadeMae") == 0) {
        // idade mae precisa tratar valores nulos com -1
        if (strlen(fieldValue) == 0) {
            r->idadeMae = -1;
        } else {
            r->idadeMae = *(int *) fieldValue;
        }
    } else if (strcmp(fieldName, "dataNascimento") == 0) {
        // dataNascimento precisa tratar valores nulos

        if (strlen(fieldValue) == 0) {
            r->dataNascimento[0] = '\0';
        } else {
            strcpy(r->dataNascimento, fieldValue);
        }
    } else if (strcmp(fieldName, "sexoBebe") == 0) {
        // sexoBebe precisa tratar valores nulos

        if (strlen(fieldValue) == 0) {
            r->sexoBebe = '0';
        } else {
            strcpy(&r->sexoBebe, fieldValue);
        }
    } else if (strcmp(fieldName, "estadoMae") == 0) {
        // estadoMae precisa tratar valores nulos

        if (strlen(fieldValue) == 0) {
            r->estadoMae[0] = '\0';
        } else {
            strcpy(r->estadoMae, fieldValue);
        }
    } else if (strcmp(fieldName, "estadoBebe") == 0) {
        // estadoBebe precisa tratar valores nulos

        if (strlen(fieldValue) == 0) {
            r->estadoBebe[0] = '\0';
        } else {
            strcpy(r->estadoBebe, fieldValue);
        }
    } else {
        fprintf(stderr, "Nome do campo inválido: %s", fieldName);
    }
}

RegistroHeader *initRegisterHeader() {
    /*
     * Inicializa um registro de cabeçalho.
     * */

    RegistroHeader *rh = calloc(1, sizeof(RegistroHeader));
    rh->status = '1';
    return rh;
}

void writeHeaderRegister(FILE *fp, RegistroHeader *rh) {
    /*
     * Escreve o registro de cabeçalho no arquivo.
     * */

    fseek(fp, 0, SEEK_SET);

    fwrite(&rh->status, sizeof(char), 1, fp);
    fwrite(&rh->RRNproxRegistro, sizeof(int), 1, fp);

    fwrite(&rh->numeroRegistrosInseridos, sizeof(int), 1, fp);
    fwrite(&rh->numeroRegistrosRemovidos, sizeof(int), 1, fp);
    fwrite(&rh->numeroRegistrosAtualizados, sizeof(int), 1, fp);

    // fill the rest with trash
    char t[111];
    memset(t, '$', 111);
    fwrite(&t, sizeof(char), 111, fp);
}

void internalWriteRegisterRRN(FILE *fp, Registro *r, int RRN, REGISTER_OPERATION op) {
    /*
     * Função interna para escrever um registro em qualquer posição de RRN.
     *
     * Caso REGISTER_OPERATION seja REGISTER_OVERWRITE, todos os dados do registro serão sobre escrevidos e os campos
     * vagos serão preenchidos com lixo.
     *
     * Caso REGISTER_OPERATION seja REGISTER_UPDATE, os campos serão sobreescrevidos, porém, os campos de tamanho
     * fixo não serão completados com lixo.
     * */

    // define trash
    char t[128];
    memset(t, '$', 128);

    // Go to the RRN position
    fseek(fp, 128 + (RRN * 128), SEEK_SET);

    // Write all fields

    // Write the length of dynamic fields
    fwrite(&r->cidadeMae_size, sizeof(int), 1, fp);
    fwrite(&r->cidadeBebe_size, sizeof(int), 1, fp);

    // Write the dynamic fields
    fwrite(r->cidadeMae, sizeof(char), r->cidadeMae_size, fp);
    fwrite(r->cidadeBebe, sizeof(char), r->cidadeBebe_size, fp);

    // skip the remaining space or fill it with trash depending on the operation
    if (op == REGISTER_UPDATE)
        fseek(fp, 97 - r->cidadeBebe_size - r->cidadeMae_size, SEEK_CUR);
    else if (op == REGISTER_OVERWRITE)
        fwrite(&t, sizeof(char), 97 - r->cidadeBebe_size - r->cidadeMae_size, fp);

    // idNascimento não precisa tratar pois nunca é nulo
    fwrite(&r->idNascimento, sizeof(int), 1, fp);

    // Tratamento da idadeMae
    if (r->idadeMae <= 0) {
        int idadeMaeInvalida = -1;
        fwrite(&idadeMaeInvalida, sizeof(int), 1, fp);
    } else {
        fwrite(&r->idadeMae, sizeof(int), 1, fp);
    }

    // Tratamento da data de nascimento
    if (strlen(r->dataNascimento) == 0) {
        t[0] = '\0';
        fwrite(&t, sizeof(char), 10, fp);
        memset(t, '$', 128);
    } else {
        fwrite(&r->dataNascimento, sizeof(char), 10, fp);
    }

    fwrite(&r->sexoBebe, sizeof(char), 1, fp);

    // Tratamento estadoMae
    if (strlen(r->estadoMae) == 0) {
        t[0] = '\0';
        fwrite(&t, sizeof(char), 2, fp);
        memset(t, '$', 128);
    } else {
        fwrite(&r->estadoMae, sizeof(char), 2, fp);
    }

    // Tratamento estadoBebe
    if (strlen(r->estadoBebe) == 0) {
        t[0] = '\0';
        fwrite(&t, sizeof(char), 2, fp);
        memset(t, '$', 128);
    } else {
        fwrite(&r->estadoBebe, sizeof(char), 2, fp);
    }

}

void addRegister(FILE *fp, Registro *r, RegistroHeader *rh) {
    /*
     * Adiciona um registro no final do arquivo.
     * */

    // Set the register status to 0 (busy)
    rh->status = '0';
    writeHeaderRegister(fp, rh);

    // Go to the next RRN position
    int RRN = rh->RRNproxRegistro;

    internalWriteRegisterRRN(fp, r, RRN, REGISTER_OVERWRITE);

    // Update the header
    rh->status = '1';
    rh->RRNproxRegistro++;
    rh->numeroRegistrosInseridos++;
    writeHeaderRegister(fp, rh);
}

void updateRegister(FILE *fp, int RRN, Registro *r, RegistroHeader *rh) {
    /*
     * Atualiza o registro em RRN.
     * */

    // Set the register status to 0 (busy)
    rh->status = '0';
    writeHeaderRegister(fp, rh);

    internalWriteRegisterRRN(fp, r, RRN, REGISTER_UPDATE);

    // Update the header
    rh->status = '1';
    rh->numeroRegistrosAtualizados++;
    writeHeaderRegister(fp, rh);
}

void insertRegister(FILE *fp, Registro *r, RegistroHeader *rh) {
    /*
     * Insere um registro novo.
     * */

    // Set the register status to 0 (busy)
    rh->status = '0';
    writeHeaderRegister(fp, rh);

    // Go to the next RRN position
    int RRN = rh->RRNproxRegistro;

    internalWriteRegisterRRN(fp, r, RRN, REGISTER_OVERWRITE);

    // Update the header
    rh->status = '1';
    rh->RRNproxRegistro++;
    rh->numeroRegistrosInseridos++;
    writeHeaderRegister(fp, rh);
}

void deleteRegister(FILE *fp, int RRN, RegistroHeader *rh) {
    /*
     * Marca o registro como removido.
     * */

    // Set the register status to 0 (busy)
    rh = readRegisterHeader(fp);
    rh->status = '0';
    writeHeaderRegister(fp, rh);
    int i = -1;
    fseek(fp, 128 + (RRN * 128), SEEK_SET);
    fwrite(&i, sizeof(int), 1, fp);

    // Update the header
    rh->status = '1';
    rh->numeroRegistrosRemovidos++;
    rh->numeroRegistrosInseridos--;
    writeHeaderRegister(fp, rh);
}

Registro *readRegister(FILE *fp, int RRN) {
    /*
     * Retorna o registro no RRN especificado ou NULL, caso o registro tenha sido removido (ou é inválido).
     * */

    // Caso integridade seja diferente de 0, o registro é invalidado
    int integridade = 0;

    // Go to the next RRN position
    fseek(fp, 128 + (RRN * 128), SEEK_SET);
    Registro *r = initRegister();

    // Read all fields

    fread(&r->cidadeMae_size, sizeof(int), 1, fp);
    fread(&r->cidadeBebe_size, sizeof(int), 1, fp);

    if (r->cidadeMae_size >= 0) {
        // tamanho alocado da string é 1 a mais para caber o \0
        r->cidadeMae = calloc(1, (r->cidadeMae_size + 1) * sizeof(char));
        fread(r->cidadeMae, sizeof(char), r->cidadeMae_size, fp);
        if (r->cidadeMae_size > 0)
            integridade++;
    } else {
        // caso seja negativo (-1) o registro foi removido
        free(r);
        return NULL;
    }

    if (r->cidadeBebe_size >= 0) {
        // tamanho alocado da string é 1 a mais para caber o \0
        r->cidadeBebe = calloc(1, (r->cidadeBebe_size + 1) * sizeof(char));
        fread(r->cidadeBebe, sizeof(char), r->cidadeBebe_size, fp);
        if (r->cidadeBebe_size > 0)
            integridade++;
    } else {
        r->cidadeBebe = "\0";
    }

    // Skip the remaining space of trash
    fseek(fp, 97 - r->cidadeBebe_size - r->cidadeMae_size, SEEK_CUR);

    fread(&r->idNascimento, sizeof(int), 1, fp);
    if (r->idNascimento != 0) integridade++;

    fread(&r->idadeMae, sizeof(int), 1, fp);
    if (r->idadeMae != 0) integridade++;

    fread(&r->dataNascimento, sizeof(char), 10, fp);
    if (strcmp(r->dataNascimento, "\0") != 0) integridade++;

    fread(&r->sexoBebe, sizeof(char), 1, fp);
    if (r->sexoBebe != '\0') integridade++;

    fread(&r->estadoMae, sizeof(char), 2, fp);
    if (strcmp(r->estadoMae, "\0") != 0) integridade++;

    fread(&r->estadoBebe, sizeof(char), 2, fp);
    if (strcmp(r->estadoBebe, "\0") != 0) integridade++;

    if (integridade == 0) {
        if (r->cidadeBebe_size > 0) free(r->cidadeBebe);
        if (r->cidadeMae_size > 0) free(r->cidadeMae);
        free(r);
        return NULL;
    }

    return r;
}

RegistroHeader *readRegisterHeader(FILE *fp) {
    /*
     * Retorna o registro de cabeçalho do arquivo.
     * */

    fseek(fp, 0, SEEK_SET);

    RegistroHeader *rh = initRegisterHeader();

    fread(&rh->status, sizeof(char), 1, fp);
    fread(&rh->RRNproxRegistro, sizeof(int), 1, fp);
    fread(&rh->numeroRegistrosInseridos, sizeof(int), 1, fp);
    fread(&rh->numeroRegistrosRemovidos, sizeof(int), 1, fp);
    fread(&rh->numeroRegistrosAtualizados, sizeof(int), 1, fp);

    return rh;
}

void printRegister(Registro *r) {
    /*
     * Printa o registro para stderr para debugar.
     * */

    fprintf(stderr, "Registro: %s (%d) | %s (%d) | %d | %d | %s | %c | %s | %s |\n", r->cidadeMae, r->cidadeMae_size,
            r->cidadeBebe, r->cidadeBebe_size, r->idNascimento, r->idadeMae,
            r->dataNascimento, r->sexoBebe, r->estadoMae, r->estadoBebe);
}

void printHeaderRegister(RegistroHeader *rh) {
    /*
     * Printa o registro de cabeçalho para stderr para debugar.
     * */

    fprintf(stderr, "Registro de cabeçalho: %c | Próximo RRN: %d | Inserido: %d | Removidos: %d | Atualizados: %d |\n",
            rh->status, rh->RRNproxRegistro, rh->numeroRegistrosInseridos, rh->numeroRegistrosRemovidos,
            rh->numeroRegistrosAtualizados);
}

void freeRegister(Registro **r) {
    /*
     * Free no registro, porém ainda é necessário dar free individualmente nos campos de tamanho variavel se eles foram
     * usados.
     * */

    free(*r);
    *r = NULL;
}
