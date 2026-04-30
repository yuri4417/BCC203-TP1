#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Arquivos.h"
#include "Struct.h"


void geraString(char *string, int tamString) { // preenche a string de maneira aleatória6
    for (int i = 0; i < tamString; i++) {
        string[i] = 'a'+ rand() % 26; // gera um caractere entre 'a' e 'z'
    }
    string[tamString] ='\0'; // termina a string
}

void geraAscendente(int qtdTotal, FILE* arq, int printFlag) {
    TipoItem *temp = calloc(1, sizeof(TipoItem));
    for (int i = 0; i < qtdTotal; i++) {
        temp->chave = (i+1);
        temp->dado1 = rand();
        geraString(temp->dado2, 1000);
        geraString(temp->dado3, 5000);
        if (printFlag)
            printf("Chave: %d, Dado1: %ld\n", temp->chave, temp->dado1);    
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
            printf("Chave: %d, Dado1: %ld\n", temp->chave, temp->dado1);
        
        fwrite(temp, sizeof(TipoItem), 1, arqDesc);
    }
    fclose(arqDesc);
    free(temp);
}

void geraRandom (int qtdTotal, FILE* arqRef, int printFlag){ 
    TipoItem *item1 = calloc(1, sizeof(TipoItem));
    TipoItem *item2 = calloc(1, sizeof(TipoItem));
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
            printf("Chave: %d, Dado1: %ld\n", item2->chave, item2->dado1);
    }
    free(item1);
    free(item2);
}

void criaArquivo(int qtdTotal, int situacao, int printFlag) {
    FILE *arqRef = fopen("arqAscendente.bin", "w+b");
    switch (situacao) {
        case 1: // Arquivo ascendente        
            geraAscendente(qtdTotal, arqRef, printFlag);
            break;
        case 2: // Arquivo descendente
            FILE *arqDesc = fopen("arqDescendente.bin", "wb");
            if (!arqDesc) {
                printf("Erro ao abrir o arquivo");
                fclose(arqRef);
                return;
            }
            geraDescendente (qtdTotal, arqDesc, printFlag);
            break;
        case 3:
        //gera o arquivo aletorio
            geraAscendente(qtdTotal, arqRef, printFlag);
            geraRandom (qtdTotal, arqRef, printFlag);
            break;
    }
    fclose(arqRef);
    if (situacao == 3)
        rename("arqAscendente.bin", "arqAleatorio.bin");
}