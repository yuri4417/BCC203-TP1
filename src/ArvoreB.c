#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "ArvoreB.h"
#include "Struct.h"

/*
void Inicializa (TipoApontador Arvore) {
    Arvore = NULL;
}
*/

void Pesquisa(TipoRegistro *x, TipoApontador Ap){
    long i = 1;
    if (Ap == NULL) {
        printf("\nChave nao encontrado!\n");
        return;
    } 
    while (i < Ap->n && x->chave > Ap->r[i-1].chave) i++;
    if(x->chave == Ap->r[i-1].chave) {
        *x = Ap->r[i-1];
        printf("\nChave %d encontrado!\n", x->chave);
        return;
    }
    if (x->chave < Ap->r[i-1].chave) 
        Pesquisa(x, Ap->p[i-1]);
    else 
        Pesquisa(x, Ap->p[i]);
}

void Imprime(TipoApontador arvore){
    int i = 0;
    if (arvore == NULL) return;
    while (i <= arvore->n) {
        Imprime(arvore->p[i]);
    if (i != arvore->n)
        printf("%d ",arvore->r[i].chave);
    i++;
    }
}

void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir){
    bool NaoAchouPosicao;
    int k;
    k = Ap->n;
    NaoAchouPosicao = (k > 0);
    while (NaoAchouPosicao) {
        if (Reg.chave >= Ap->r[k-1].chave) {
            NaoAchouPosicao = false;
            break;
        }
        Ap->r[k] = Ap->r[k-1];
        Ap->p[k+1] = Ap->p[k];
        k--;
        if (k < 1) NaoAchouPosicao = false;
    }
    Ap->r[k] = Reg;
    Ap->p[k+1] = ApDir;
    Ap->n++;
}

void Ins(TipoRegistro Reg, TipoApontador Ap, bool *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno){
    long i =1; long j;
    TipoApontador ApTemp;
    if (Ap == NULL) {
        *Cresceu = true;
        RegRetorno->chave = Reg.chave;
        (*ApRetorno) = NULL;
        return;
    }
    while (i < Ap->n && Reg.chave > Ap->r[i-1].chave) i++;
    if (Reg.chave == Ap->r[i-1].chave) {
        printf("Erro: Registro ja existente\n");
        *Cresceu = false;
        return;
    }
    if (i > 1 && Reg.chave < Ap->r[i-1].chave) i--;
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno);
    if (!*Cresceu) return;
    if (Ap->n < 2*M) {
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
        *Cresceu = false;
        return;
    }
    ApTemp = (TipoApontador) malloc(sizeof(TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;
    if (i < M + 1) {
        InsereNaPagina(ApTemp, Ap->r[2*M-1], Ap->p[2*M]);
        Ap->n--;
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno);
    } else InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno);
    for (j = M + 2; j <= 2*M; j++)
        InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j]);
    Ap->n = M;
    ApTemp->p[0] = Ap->p[M+1];
    *RegRetorno = Ap->r[M];
    *ApRetorno = ApTemp;
    
}

void Insere(TipoRegistro Reg, TipoApontador *Ap) {
    bool Cresceu;
    TipoRegistro RegRetorno;
    TipoApontador ApRetorno, ApTemp;
    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno);
    if (Cresceu) {
        ApTemp = (TipoPagina *) malloc(sizeof(TipoPagina));
        if(!ApTemp) {
            printf("Erro ao alocar memoria\n");     
            return;
        }
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap;
        *Ap = ApTemp;
        // free(ApTemp);  
    }   
}

void arvoreB(int chave, int situacao) {
    TipoApontador pArvore = NULL;
    FILE* pArq = NULL;
    if (situacao == 1)
        pArq = fopen("arqAscendente.bin", "rb");
    else if (situacao == 2)
        pArq = fopen("arqDescendente.bin", "rb");
    else  
        pArq = fopen("arqAleatorio.bin", "rb");
    if (!pArq) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }

    TipoItem temp = {0};
    while (fread(&temp, sizeof(TipoItem), 1, pArq) == 1) {
        Insere(temp, &pArvore);
    }
    TipoRegistro busca = {0};
    busca.chave = chave;

    Pesquisa(&busca, pArvore);
    fclose(pArq);
}