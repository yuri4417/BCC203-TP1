#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Arquivos.h"
#include "Struct.h"

#define cresc1M "./data/arqCresc-1M.bin"
#define desc1M "./data/arqDesc-1M.bin"
#define rand1M "./data/arqRand-1M.bin"

//Abre em rb para verificar se o arquivo existe
int validaArquivo(char *filePath, FILE** pTemp) {
    *pTemp = fopen(filePath, "rb");
    return (*pTemp != NULL);
}

//Impressao de arquivo quando ele ja existe
void printFile(FILE *pArq, int tam) {
    //Buffer para leitura em blocos
    TipoItem *buffer = malloc(sizeof(TipoItem) * BLOCK_SIZE);
    if (!buffer)
        return; 

    int toRead;
    printf("Chaves presentes no arquivo:\n");
    //Le o arquivo em blocos e imprime cada chave
    while (tam > 0) {
        toRead = (tam > BLOCK_SIZE) ? BLOCK_SIZE : tam;
        int lidos = fread(buffer, sizeof(TipoItem), toRead, pArq);
        for (int i = 0; i < lidos; i++) 
            printf("%d ", buffer[i].chave);
        tam -= lidos;
    }
    printf("\n");
    free(buffer);
}

//Gera um arquivo em ordem crescente ou decrescente
void geraOrdenado(FILE** pArq, char* filePath, int printFlag, int countChave, int chaveSum, long desloc, int tam) {
    //Valida o arquivo e imprime se necessario
    if (validaArquivo(filePath, pArq)) {
        //Posiciona o ponteiro para imprimir corretamente
        fseek(*pArq, desloc, SEEK_SET);
        if (printFlag) {
            printFile(*pArq, tam);
            fseek(*pArq, desloc, SEEK_SET);
        }
        return;
    }
    
    //Cria arquivo e aloca o buffer
    *pArq = fopen(filePath, "w+b");
    if (!*pArq)
        return;
    TipoItem *buffer = calloc(BLOCK_SIZE, sizeof(TipoItem));
    if (!buffer)
        return;

    int qtdRestante = MAXTAM;
    int toWrite;
    
    //Gera o arquivo em blocos na memoria principal e escreve no arquivo
    while (qtdRestante > 0) {
        toWrite = (qtdRestante > BLOCK_SIZE) ? BLOCK_SIZE : qtdRestante;
        for (int i = 0; i < toWrite; i++) {
            buffer[i].chave = countChave;
            buffer[i].dado1 = rand();
            sprintf(buffer[i].dado2, "%d %ld", buffer[i].chave, buffer[i].dado1);
            sprintf(buffer[i].dado3, "Chave %d dado long %ld", buffer[i].chave, buffer[i].dado1);
            //Se o arquivo nao existir ele printa as chaves
            if (printFlag)
                printf("Chave: %d ", buffer[i].chave);
            countChave += chaveSum;
        }
        fwrite(buffer, sizeof(TipoItem), toWrite, *pArq);
        qtdRestante -= toWrite;
    }
    //Volta o ponteiro para a posicao inicial
    fseek(*pArq, desloc, SEEK_SET);
    free(buffer);
}

//Gera arquivo aleatorio
void geraRandom (FILE** arqRef, int printFlag, int tam) { 
    char fileName[50] = {0};
    //Cria o caminho do arquivo e abre ele
    if (tam == MAXTAM)
        strcpy(fileName, rand1M);
    else
        sprintf(fileName, "./data/arqRand-%d.bin", tam);

    *arqRef = fopen(fileName, "r+b");
    if (*arqRef != NULL) {
        if (printFlag) 
            printFile(*arqRef, tam);
        return; 
    }

    //Abre/cria o arquivo de referencia
    FILE *pFileCopy = NULL;
    if (!validaArquivo(cresc1M, &pFileCopy)) {
        geraOrdenado(&pFileCopy, cresc1M, printFlag, 1, 1, 0, tam);
    }
    if (!pFileCopy) 
        return;

    //Carregamento do arquivo para memoria principal
    TipoItem *buffer = malloc(tam * sizeof(TipoItem));
    if (!buffer) {
        fclose(pFileCopy);
        return; 
    }
    fread(buffer, sizeof(TipoItem), tam, pFileCopy);
    fclose(pFileCopy);
    
    //Embaralhamento do arquivo
    TipoItem temp = {0};
    if (printFlag) 
        printf("Chaves presentes no arquivo:\n");
    for (int i = 0; i < tam; i++) {
        int posTroca = i + rand() % (tam - i);

        temp = buffer[i];
        buffer[i] = buffer[posTroca];
        buffer[posTroca] = temp;

        if (printFlag)
            printf("%d ", buffer[i].chave);
    }
    if (printFlag)
        printf("\n");

    //Escreve no arquivo de destino
    *arqRef = fopen(fileName, "w+b");
    if (*arqRef) {
        fwrite(buffer, sizeof(TipoItem), tam, *arqRef);
        rewind(*arqRef); 
    }

    free(buffer);
}

//Cria o arquivo de acordo com a situacao passada
FILE* criaArquivos(int situacao, int printFlag, int tam) {
    FILE *pArq = NULL;

    //Cria a pasta data se ela nao existir
    struct stat st = {0};
    if (stat("./data", &st) == -1)
        mkdir("./data", 0777);

    //Decide qual arquivo gerar
    switch (situacao) {
        case 1:
            geraOrdenado(&pArq, cresc1M, printFlag, 1, 1, 0, tam);
            break;
        case 2:
            geraOrdenado(&pArq, desc1M, printFlag, MAXTAM, -1, sizeof(TipoItem) * (MAXTAM - tam), tam);
            break;
        case 3:
            geraRandom(&pArq, printFlag, tam);
            break;

    }
    return pArq;
}
