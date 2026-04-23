#ifndef PESQUISAS_H
#define PESQUISAS_H
#include <stdio.h>

#define ITENSPAGINA 4
#define MAXTABELA 100

typedef struct{
    int chave;
    int posicao;
}TipoIndice;

typedef struct{
    int chave;
    long int dado1;
    char dado2[1001];
    char dado3[5001];
}TipoItem;

int acessoIndexado(TipoIndice tabela[], int tam, TipoItem *item, FILE *arq);

#endif