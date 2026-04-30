#ifndef ACESSOIDX_H
#define ACESSOIDX_H

#include "Struct.h"
int comparaCrescente(int valor, int chave);
int comparaDecrescente(int valor, int chave);
int acessoIndexado(TipoIndice tabela[],TipoItem *item, int situacao);
int pesquisaBinaria(TipoItem *v, int esq, int dir, int chave, int (*compara)(int, int));
#endif