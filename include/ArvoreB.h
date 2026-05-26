#ifndef ARVOREB_H
#define ARVOREB_H
#include <stdbool.h>
#include "Struct.h"
void Pesquisa(TipoRegistro *x, TipoApontador Ap,Bench *bench);
void Imprime(TipoApontador arvore);
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir, Bench *bench);
void Ins(TipoRegistro Reg, TipoApontador Ap, bool *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno,Bench *bench,bool *memCheia);
void Insere(TipoRegistro Reg, TipoApontador *Ap,Bench *bench,bool *memCheia);
void arvoreB(int chave, Bench *bench, int tam, FILE* pArq);
void LiberaArvore(TipoApontador arvore);
#endif //ARVOREB_H
    