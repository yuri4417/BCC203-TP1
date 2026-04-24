#include <stdio.h>
#include "Arquivos.h"
#include "Struct.h"


void geraString(char *string, int tamString) { // preenche a string de maneira aleatória6
    for (int i = 0; i < tamString; i++) {
        string[i] = 'a'+ rand()%26; // gera um caractere entre 'a' e 'z'
    }
    string[tamString] ='\0'; // termina a string
}

void geraAscendente(int qtdTotal, FILE* arq) {
    TipoItem temp;
    for (int i = 0; i < qtdTotal; i++) {
        temp.chave = (i+1);
        temp.dado1 = rand();
        geraString(temp.dado2, 1000);
        geraString(temp.dado3, 5000);
        fwrite(&temp, sizeof(TipoItem), 1, arq);
    }
}

void criaArquivo(int qtdTotal, int situacao) {
    TipoItem temp;
    FILE *arqRef = fopen("arqAscendente.bin", "wb");
    switch (situacao) {
        case 1: // Arquivo ascendente        
            geraAscendente(qtdTotal, arqRef);
            break;
        case 2: // Arquivo descendente
            FILE *arqDesc = fopen("arqDescendente.bin", "wb");
            if (!arqDesc)
                return;

            for (int i = qtdTotal; i > 0; i--) {
                temp.chave = i;
                temp.dado1 = rand();
                geraString(temp.dado2, 1000);
                geraString(temp.dado3, 5000);

                fwrite(&temp, sizeof(TipoItem), 1, arqDesc);
            }

            fclose(arqDesc);
            break;
        case 3:
            geraAscendente(qtdTotal, arqRef);
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
            }
            break;
    }
    fclose(arqRef);
}