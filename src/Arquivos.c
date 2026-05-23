#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "Arquivos.h"
#include "Struct.h"

#define MAXTAM 1000000
#define cresc1M "./data/arqCresc-1M.bin"
#define desc1M "./data/arqDesc-1M.bin"
#define rand1M "./data/arqRand-1M.bin"

int validaArquivo(char *filePath, FILE** pTemp) {
    *pTemp = fopen(filePath, "rb");
    return (*pTemp != NULL);
}






void geraOrdenado(FILE** pArq, char* filePath, int printFlag, int countChave, int chaveSum, long desloc) {
    if (validaArquivo(filePath, pArq)) {
        fseek(*pArq, desloc, SEEK_SET);
        return;
    }
    
    *pArq = fopen(filePath, "w+b");
    if (!*pArq)
        return;
    TipoItem *buffer = malloc(sizeof(TipoItem) * BLOCK_SIZE);
    if (!buffer)
        return;

    int qtdRestante = MAXTAM;
    int toWrite;
    while (qtdRestante > 0) {
        toWrite = (qtdRestante > BLOCK_SIZE) ? BLOCK_SIZE : qtdRestante;
        for (int i = 0; i < toWrite; i++) {
            buffer[i].chave = countChave;
            buffer[i].dado1 = rand();
            sprintf(buffer[i].dado2, "%d %ld", buffer[i].chave, buffer[i].dado1);
            sprintf(buffer[i].dado3, "Chave %d dado long %ld", buffer[i].chave, buffer[i].dado1);
            if (printFlag)
                printf("Chave: %d ", buffer[i].chave);
            countChave += chaveSum;
        }
        fwrite(buffer, sizeof(TipoItem), toWrite, *pArq);
        qtdRestante -= toWrite;
    }
    fseek(*pArq, desloc, SEEK_SET);
    free(buffer);
}

void geraRandom (int tam, FILE** arqRef, int printFlag) { 
    char fileName[50];
    if (tam == MAXTAM)
        strcpy(fileName, rand1M);
    else
        sprintf(fileName, "./data/arqRand-%d.bin", tam);

    *arqRef = fopen(fileName, "r+b");
    if (*arqRef != NULL) {
        return; 
    }

    FILE *pFileCopy;
    if (!validaArquivo(cresc1M, &pFileCopy)) {
        pFileCopy = fopen(cresc1M, "w+b");
        if (pFileCopy)
            geraOrdenado(arqRef, cresc1M, printFlag, 0, 1, 0);
    }
    
    if (!pFileCopy) 
        return;

    TipoItem *buffer = malloc(tam * sizeof(TipoItem));
    if (!buffer) {
        fclose(pFileCopy);
        return; 
    }

    fread(buffer, sizeof(TipoItem), tam, pFileCopy);
    fclose(pFileCopy);
    TipoItem temp = {0};
    for (int i = 0; i < tam; i++) {
        int posTroca = i + rand() % (tam - i);

        temp = buffer[i];
        buffer[i] = buffer[posTroca];
        buffer[posTroca] = temp;

        if (printFlag)
            printf("Chave: %d ", buffer[i].chave);
    }

    *arqRef = fopen(fileName, "w+b");
    if (*arqRef) {
        fwrite(buffer, sizeof(TipoItem), tam, *arqRef);
        rewind(*arqRef); 
    }

    free(buffer);
}



FILE* criaArquivos(int situacao, int printFlag, int tam) { // Cria tres arquivos de 1M
    FILE *pArq = NULL;

    struct stat st = {0};
    if (stat("./data", &st) == -1)
        mkdir("./data", 0777);

    switch (situacao) {
        case 1:
            geraOrdenado(&pArq, cresc1M, printFlag, 0, 1, 0);
            break;
        case 2:
            geraOrdenado(&pArq, desc1M, printFlag, MAXTAM, -1, sizeof(TipoItem) * (MAXTAM - tam));
            break;
        case 3:
            geraRandom(tam, &pArq, printFlag);
            break;

    }
    return pArq;
}
