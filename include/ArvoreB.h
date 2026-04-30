#ifndef ARVOREB_H
#define ARVOREB_H

void Inicializa (TipoApontador Arvore);
void Pesquisa(TipoRegistro *x, TipoApontador Ap);
void Imprime(TipoApontador arvore);
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir);
void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno);
void Insere(TipoRegistro Reg, TipoApontador *Ap);
#endif //ARVOREB_H
    