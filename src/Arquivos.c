#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Arquivos.h"
#include "Struct.h"

#define MAXTAM 1000000
#define BLOCK_SIZE 175000
void geraString(char *string, int tamString) { // preenche a string de maneira aleatória6
    for (int i = 0; i < tamString; i++) {
        string[i] = 'a'+ rand() % 26; // gera um caractere entre 'a' e 'z'
    }
    string[tamString] ='\0'; // termina a string
}


void geraAscendente(int qtdTotal, FILE* arq, int printFlag) {
    TipoItem temp = {0};
    for (int i = 0; i < qtdTotal; i++) {
        temp.chave = (i+1);
        temp.dado1 = rand();
        //geraString(temp.dado2, 1000);
        sprintf(temp.dado2,"%d %ld", temp.chave, temp.dado1);
        sprintf(temp.dado3,"%d %ld", temp.chave, temp.dado1);
        
        //geraString(temp.dado3, 5000);
        if (printFlag)
            printf(" Chave: %d ", temp.chave);    
        fwrite(&temp, sizeof(TipoItem), 1, arq);
    }
}

void geraDescendente (int qtdTotal, FILE *arqDesc, int printFlag){
    
    TipoItem temp = {0};
    for (int i = qtdTotal; i > 0; i--) {
        temp.chave = i;
        temp.dado1 = rand();
        //geraString(temp.dado2, 1000);
        //geraString(temp.dado3, 5000);
        sprintf(temp.dado2,"%d %ld", temp.chave, temp.dado1);
        sprintf(temp.dado3,"Chave %d dado long %ld", temp.chave, temp.dado1);
        if (printFlag)
            printf(" Chave: %d", temp.chave);
        
        fwrite(&temp, sizeof(TipoItem), 1, arqDesc);
    }
    fclose(arqDesc);
}

void geraRandom (int qtdTotal, FILE** arqRef, int printFlag){ 
    TipoItem *copy = calloc(BLOCK_SIZE, sizeof(TipoItem));
    if (!copy)
        return;
    int qtdRestante= qtdTotal;
    int lidos;
    int qtdLer;
    while (qtdRestante > 0) {
        qtdLer = (qtdRestante > BLOCK_SIZE) ? BLOCK_SIZE : qtdRestante;
        lidos = fread(copy, sizeof(TipoItem), qtdLer, *arqRef);
        //TODO: criacao do novo arquivo (string customizada) + copia para ele
    }
    
    
    
    TipoItem item1 = {0};
    TipoItem item2 = {0};
    int posTroca;
    for (int i = 0; i < qtdTotal; i++) {
        posTroca = i + rand() % (qtdTotal - i);

        fseek(*arqRef, i * sizeof(TipoItem), SEEK_SET);
        fread(&item1, sizeof(TipoItem), 1, *arqRef);

        fseek(*arqRef, posTroca * sizeof(TipoItem), SEEK_SET);
        fread(&item2, sizeof(TipoItem), 1, *arqRef);   

        fseek(*arqRef, i * sizeof(TipoItem), SEEK_SET);
        fwrite(&item2, sizeof(TipoItem), 1, *arqRef);

        fseek(*arqRef, posTroca * sizeof(TipoItem), SEEK_SET);
        fwrite(&item1, sizeof(TipoItem), 1, *arqRef);
        if(printFlag)
            printf(" Chave: %d ", item2.chave);
    }
    printf("\n");
    //TODO: atualizacao de parq
}

int validaArquivo(char *filePath, FILE** pTemp) {
    *pTemp = fopen(filePath, "rb");
    return (*pTemp != NULL);
}


FILE* criaArquivos(int situacao, int printFlag) { // Cria tres arquivos de 1M
    static FILE *pArq;
    switch (situacao) {
        case 1:
            if (!validaArquivo("./data/arqCresc-1M.bin", &pArq)) {
                pArq = fopen("./data/arqCresc-1M.bin", "w+b");
                geraAscendente(MAXTAM, pArq, printFlag);

            }
            break;
        case 2:
            if (!validaArquivo("./data/arqCresc-1M.bin", &pArq)) {
                pArq = fopen("./data/arqCresc-1M.bin", "w+b");
                geraDescendente(MAXTAM, pArq, printFlag);
            }
            break;
        case 3:
            //TODO: Verificação se o arquivo rand desejado já está gerado na pasta data
            if (!validaArquivo("./data/arqRand-1M.bin", &pArq)) {
                pArq = fopen("./data/arqRand-1M.bin", "w+b");
                geraRandom(MAXTAM, &pArq, printFlag);
            }
            break;

    }

    fseek(pArq, 0, SEEK_SET);
    return pArq;
}
