
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "Executavel.h"
#include "ArvoreBEstrela.h"
#include "Struct.h"

void LiberaArvore(TipoApontadorEstrela arvore) {
    if (arvore == NULL)
        return;
    for (int i = 0; i <= arvore->n; i++) {
        LiberaArvore(arvore->p[i]);
    }
    free(arvore);
}

void Pesquisa(TipoRegistro *x, TipoApontadorEstrela Ap, Bench *bench)
{
    int i;
    TipoApontadorEstrela Pag;
    Pag = Ap;
    
    if (Ap->Pt == Interna)
    {
        i = 1;
        while (i < Pag->UU.U0.ni && x->chave > Pag->UU.U0.ri[i - 1]){
            i++;
            bench->comp++;
        } 
        bench->comp++;
        bench->comp++;
        if (x->chave < Pag->UU.U0.ri[i - 1])
            Pesquisa(x, Pag->UU.U0.pi[i - 1], bench);
        else Pesquisa(x, Pag->UU.U0.pi[i], bench);
        return;
    }
    i = 1;
    while (i < Pag->UU.U1.ne && x->chave > Pag->UU.U1.re[i - 1].chave){
        i++;
        bench->comp++;
    }
    bench->comp++;
    bench->comp++;
    if (x->chave == Pag->UU.U1.re[i - 1].chave){
        *x = Pag->UU.U1.re[i - 1];
        printItem(x);
    }
    else 
        printf("Item nao esta presente na arvore\n");
}


void InsereNaPagina(TipoApontadorEstrela Ap, TipoRegistro Reg, TipoApontadorEstrela ApDir, Bench *bench){
    int k;
    if(Ap->Pt == Interna){
        k = Ap->UU.U0.ni;
        while(k>0){
            if (Reg.chave >= Ap->UU.U0.ri[k-1]) {
                bench->comp++;
                break;
            }
            Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k-1];
            Ap->UU.U0.pi[k+1] = Ap->UU.U0.pi[k];
            k--;
        }
        Ap->UU.U0.ri[k] = Reg.chave;
        Ap->UU.U0.pi[k+1] = ApDir;
        Ap->UU.U0.ni++;
        
    }    
    else{
        k = Ap->UU.U1.ne;
      
        while(k>0){
            if (Reg.chave >= Ap->UU.U1.re[k-1].chave) {
                bench->comp++;
                break;
            }
            Ap->UU.U1.re[k] = Ap->UU.U1.re[k-1];
            k--;
        }
        Ap->UU.U1.re[k] = Reg;
        Ap->UU.U1.ne++;
    }
    
    
}

void Ins(TipoRegistro Reg, TipoApontadorEstrela Ap, bool *Cresceu, TipoRegistro *RegRetorno, TipoApontadorEstrela *ApRetorno, Bench *bench){
    long i =1; long j;
    TipoApontadorEstrela ApTemp;

    if (Ap == NULL) { // isso so acontece  
        
        *Cresceu = true;

        *RegRetorno = Reg;
        (*ApRetorno) = NULL;
        return;
    }
           
    if(Ap->Pt == Interna){
        while (i < Ap->UU.U0.ni && Reg.chave > Ap->UU.U0.ri[i-1]){
            i++;
            bench->comp++;
        } 
    }
    
    bench->comp++;
    if (Reg.chave == Ap->UU.U0.ri[i-1]) {
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
    ApTemp = (TipoApontadorEstrela) malloc(sizeof(TipoPaginaEstrela));
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

void Insere(TipoRegistro Reg, TipoApontadorEstrela *Ap, Bench *bench) {
    bool Cresceu;
    TipoRegistro RegRetorno;
    TipoApontadorEstrela ApRetorno, ApTemp;
    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno,bench);
    
    if (Cresceu) {        
        
        ApTemp = (TipoPaginaEstrela *) malloc(sizeof(TipoPaginaEstrela));
        if(!ApTemp) {
            printf("Erro ao alocar memoria\n");     
            return;
        }

        if(*Ap == NULL){
            ApTemp->Pt = Externa;
            ApTemp->UU.U1.ne = 1;
            ApTemp->UU.U1.re[0] = RegRetorno;
            *Ap = ApTemp;
        }
        else{
            ApTemp->Pt = Interna;
            ApTemp->UU.U0.ni = 1;
            ApTemp->UU.U0.ri[0] = RegRetorno.chave;
            ApTemp->UU.U0.pi[1] = ApRetorno;
            ApTemp->UU.U0.pi[0] = *Ap;
            *Ap = ApTemp;
        }
    }
}

void arvoreB(int chave, int situacao, Bench *bench) {
    TipoApontadorEstrela pArvore = NULL;
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