#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "Executavel.h"
#include "ArvoreB.h"
#include "Struct.h"

void LiberaArvore(TipoApontador arvore) {
    if (arvore == NULL)
        return;
    for (int i = 0; i <= arvore->n; i++) {
        LiberaArvore(arvore->p[i]);
    }
    free(arvore);
}

void Pesquisa(TipoRegistro *x, TipoApontador Ap, Bench *bench){
    long i = 1;
    if (Ap == NULL) {
        printf("\nChave nao encontrado!\n");
        return;
    } 
    
    while (i < Ap->n && x->chave > Ap->r[i-1].chave) {
        i++;
        bench->comp++;
    }
    
    bench->comp++;
    if(x->chave == Ap->r[i-1].chave) {
        *x = Ap->r[i-1];
        printItem(x);
        return;
    }
    
    bench->comp++;
    if (x->chave < Ap->r[i-1].chave) 
        Pesquisa(x, Ap->p[i-1], bench);
    else 
        Pesquisa(x, Ap->p[i], bench);
}

void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir, Bench *bench){
    int k;
    k = Ap->n;
    while (k>0) {
        if (Reg.chave >= Ap->r[k-1].chave) {
            bench->comp++;
            break;
        }
        Ap->r[k] = Ap->r[k-1];
        Ap->p[k+1] = Ap->p[k];
        k--;
    }
    Ap->r[k] = Reg;
    Ap->p[k+1] = ApDir;
    Ap->n++;
}

void Ins(TipoRegistro Reg, TipoApontador Ap, bool *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno, Bench *bench){
    long i =1; long j;
    TipoApontador ApTemp;
    if (Ap == NULL) {
        *Cresceu = true;
        *RegRetorno = Reg;
        (*ApRetorno) = NULL;
        return;
    }
    while (i < Ap->n && Reg.chave > Ap->r[i-1].chave){
        i++;
        bench->comp++;
    } 
    bench->comp++;
    if (Reg.chave == Ap->r[i-1].chave) {
        printf("Erro: Registro ja existente\n");
        *Cresceu = false;
        return;
    }
    bench->comp++;
    if (Reg.chave < Ap->r[i-1].chave) i--;
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno, bench);
    bench->comp++;
    if (!*Cresceu)  
        return;
    bench->comp++;
    if (Ap->n < 2*M) {
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno, bench);
        *Cresceu = false;
        return;
    }
    ApTemp = (TipoApontador) malloc(sizeof(TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;
    if (i < M + 1) {
        InsereNaPagina(ApTemp, Ap->r[2*M-1], Ap->p[2*M],bench);
        Ap->n--;
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno,bench);
    } else InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno,bench);
    for (j = M + 2; j <= 2*M; j++)
        InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j],bench);
    Ap->n = M;
    ApTemp->p[0] = Ap->p[M+1];
    *RegRetorno = Ap->r[M];
    *ApRetorno = ApTemp;
    
}

void Insere(TipoRegistro Reg, TipoApontador *Ap, Bench *bench) {
    bool Cresceu;
    TipoRegistro RegRetorno;
    TipoApontador ApRetorno, ApTemp;
    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno,bench);
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

    }   
}

void arvoreB(int chave, int situacao, Bench *bench) {
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
    
    TipoRegistro temp = {0};
    
    while (fread(&temp, sizeof(TipoRegistro), 1, pArq) == 1){
        Insere(temp, &pArvore, bench);
        bench->transf++;
    }
        
    TipoRegistro busca = {0};
    busca.chave = chave;
    Pesquisa(&busca, pArvore,bench);
    
    LiberaArvore(pArvore);
    fclose(pArq);
}