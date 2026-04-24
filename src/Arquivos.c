#include <stdio.h>
#include <stdlib.h>
#include "Arquivos.h"
#include "Struct.h"


void geraString(char *string, int tamString) { // preenche a string de maneira aleatória6
    for (int i = 0; i < tamString; i++) {
        string[i] = 'a'+ rand() % 26; // gera um caractere entre 'a' e 'z'
    }
    string[tamString] ='\0'; // termina a string
}

void geraAscendente(int qtdTotal, FILE* arq, int printFlag) {
    TipoItem temp;
    for (int i = 0; i < qtdTotal; i++) {
        temp.chave = (i+1);
        temp.dado1 = rand();
        geraString(temp.dado2, 1000);
        geraString(temp.dado3, 5000);
        if (printFlag)
            printf("Chave: %d, Dado1: %ld, Dado2: %s, Dado3: %s\n\n", temp.chave, temp.dado1, temp.dado2, temp.dado3);    
        fwrite(&temp, sizeof(TipoItem), 1, arq);
    }
}

void geraDescendente (int qtdTotal, FILE *arqDesc, int printFlag){
    
    TipoItem temp;
    for (int i = qtdTotal; i > 0; i--) {
        temp.chave = i;
        temp.dado1 = rand();
        geraString(temp.dado2, 1000);
        geraString(temp.dado3, 5000);
        if (printFlag)
            printf("Chave: %d, Dado1: %ld, Dado2: %s, Dado3: %s\n", temp.chave, temp.dado1, temp.dado2, temp.dado3);
        
        fwrite(&temp, sizeof(TipoItem), 1, arqDesc);
    }
    fclose(arqDesc);
}

void geraRandom (int qtdTotal, FILE* arqRef, int printFlag){
    TipoItem item1, item2;
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
        if(printFlag)// caso passar o -P vai imprimir todas as chaves e dados
            printf("Chave: %d, Dado1: %ld, Dado2: %s, Dado3: %s\n", item2.chave, item2.dado1, item2.dado2, item2.dado3);
    }
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