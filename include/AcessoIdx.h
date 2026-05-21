#ifndef ACESSOIDX_H
#define ACESSOIDX_H

#include "Struct.h"
int comparaCrescente(int valor, int chave);
int comparaDecrescente(int valor, int chave);
int acessoIndexado(TipoIndice tabela[],TipoItem *item, int situacao, Bench *bench, int tam, int printFlag);
int pesquisaBinaria(TipoItem *v, int esq, int dir, int chave, int (*compara)(int, int), Bench *bench);
#endif