#ifndef ARVOREB_H
#define ARVOREB_H
#include <stdbool.h>
#include "Struct.h"
void Inicializa (TipoApontador *Arvore);
void Pesquisa(TipoRegistro *x, TipoApontador Ap);
void Imprime(TipoApontador arvore);
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir);
void Ins(TipoRegistro Reg, TipoApontador Ap, bool *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno);
void Insere(TipoRegistro Reg, TipoApontador *Ap);
void arvoreB(int chave, int situacao);
#endif //ARVOREB_H
    