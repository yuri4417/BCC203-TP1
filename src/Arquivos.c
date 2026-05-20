#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Arquivos.h"
#include "Struct.h"

void geraAscendente(int qtdTotal, FILE* arq, int printFlag) {
    TipoItem *temp = calloc(1, sizeof(TipoItem));
    for (int i = 0; i < qtdTotal; i++) {
        temp->chave = (i+1);
        temp->dado1 = rand();
        geraString(temp->dado2, 1000);
        geraString(temp->dado3, 5000);
        if (printFlag)
            printf(" Chave: %d ", temp->chave);    
        fwrite(temp, sizeof(TipoItem), 1, arq);
    }
    free(temp);
}

void geraDescendente (int qtdTotal, FILE *arqDesc, int printFlag){
    
    TipoItem *temp = calloc(1, sizeof(TipoItem));
    for (int i = qtdTotal; i > 0; i--) {
        temp->chave = i;
        temp->dado1 = rand();
        geraString(temp->dado2, 1000);
        geraString(temp->dado3, 5000);
        if (printFlag)
            printf(" Chave: %d", temp->chave);
        
        fwrite(temp, sizeof(TipoItem), 1, arqDesc);
    }
    fclose(arqDesc);
    free(temp);
}

void geraRandom (int qtdTotal, FILE* arqRef, int printFlag){ 
    TipoItem item1 = {0};
    TipoItem item2 = {0};
    int posTroca;
    for (int i = 0; i < qtdTotal; i++) {
        posTroca = i + rand() % (qtdTotal - i);

        fseek(arqRef, i * sizeof(TipoItem), SEEK_SET);
        fread(&item1, sizeof(TipoItem), 1, arqRef);

        fseek(arqRef, posTroca * sizeof(TipoItem), SEEK_SET);
        fread(&item2, sizeof(TipoItem), 1, arqRef);   

        fseek(arqRef, i * sizeof(TipoItem), SEEK_SET);
        fwrite(&item2, sizeof(TipoItem), 1, arqRef);

        fseek(arqRef, posTroca * sizeof(TipoItem), SEEK_SET);
        fwrite(&item1, sizeof(TipoItem), 1, arqRef);
        if(printFlag)
            printf(" Chave: %d ", item2.chave);
    }
    printf("\n");
}

FILE* criaArquivos(int situacao, int printFlag) { // Cria tres arquivos de 1M
    FILE *pArq;
    switch (situacao) {
        case 1:
            pArq = fopen("./data/arqCresc-1M.bin", "rb");
            if (!pArq) {
                pArq = fopen("./data/arqCresc-1M.bin", "wb");
                geraAscendente(1000000, pArq, printFlag);
            }
            break;
        case 2:
            pArq = fopen("./data/arqDesc-1M.bin", "rb");
            if (!pArq) {
                pArq = fopen("./data/arqDesc-1M.bin", "wb");
                geraDescendente(1000000, pArq, printFlag);
            }
            break;
        case 3:
            pArq = fopen("./data/arqRand-1M.bin", "rb");
            if (!pArq) {
                pArq = fopen(".data/arqRand-1M.bin", "rb");
                geraRandom(1000000, pArq, printFlag);
            }
            break;

    }
    return pArq;
}


void geraString(char *string, int tamString) { // preenche a string de maneira aleatória6
    for (int i = 0; i < tamString; i++) {
        string[i] = 'a'+ rand() % 26; // gera um caractere entre 'a' e 'z'
    }
    string[tamString] ='\0'; // termina a string
}
