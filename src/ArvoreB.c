#include <stdio.h>
#include "ArvoreB.h"

void Inicializa (TipoApontador Arvore){
    Arvore = NULL;
}

void Pesquisa(TipoRegistro *x, TipoApontador Ap){
    long i = 1;
    if (Ap == NULL) {
        printf("Registro nao encontrado\n");
        return;
    } 
    while (i < Ap->n && x->chave > Ap->r[i-1].chave) i++;
    if(x->chave == Ap->r[i-1].chave) {
        *x = Ap->r[i-1];
        printf("Registro encontrado\n");
        return;
    }
    if (x->chave < Ap->r[i-1].chave) 
        Pesquisa(x, Ap->p[i-1]);
    else Pesquisa(x, Ap->p[i]);
}

void Imprime(TipoApontador arvore){
    int i = 0;
    if (arvore == NULL) return;
    while (i <= arvore->n) {
        Imprime(arvore->p[i]);
    if (i != arvore->n)
        printf("%d",arvore->r[i].chave);
    i++;
    }
}

void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir){
    
    int k;
    k = Ap->n;
    
    while (k>0) {
        if (Reg.chave >= Ap->r[k-1].chave) 
            break;
        
        Ap->r[k] = Ap->r[k-1];
        Ap->p[k+1] = Ap->p[k];
        k--;
        
    }
    Ap->r[k] = Reg;
    Ap->p[k+1] = ApDir;
    Ap->n++;
}

void Ins(TipoRegistro Reg, TipoApontador Ap, short *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno){
    long i =1; long j;
    TipoApontador ApTemp;
    if (Ap == NULL) {
        *Cresceu = 1;
        (*RegRetorno) = Reg;
        (*ApRetorno) = NULL;
        return;
    }
    while (i < Ap->n && Reg.chave > Ap->r[i-1].chave) i++;
    if (Reg.chave == Ap->r[i-1].chave) {
        printf("Erro: Registro ja existente\n");
        *Cresceu = 0;
        return;
    }
    if (Reg.chave < Ap->r[i-1].chave) i--;
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);
    if (!*Cresceu) return;
    if (Ap->n < MM) {
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
        *Cresceu = 0;
        return;
    }
    ApTemp = (TipoApontador) malloc(sizeof(TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;
    if (i < M + 1) {
        InsereNaPagina(ApTemp, Ap->r[MM-1], Ap->p[MM]);
        Ap->n--;
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    } else InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
    for (j = M + 2; j <= MM; j++)
        InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j]);
    Ap->n = M;
    ApTemp->p[0] = Ap->p[M+1];
    *RegRetorno = Ap->r[M];
    *ApRetorno = ApTemp;
}

void Insere(TipoRegistro Reg, TipoApontador *Ap){
    short Cresceu;
    TipoRegistro RegRetorno;
    TipoApontador ApRetorno, ApTemp;
    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
    if (Cresceu) {
        ApTemp = (TipoPagina *) malloc(sizeof(TipoPagina));
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap;

        *Ap = ApTemp;
    }
}