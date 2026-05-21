
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "Executavel.h"
#include "ArvoreBEstrela.h"
#include "Struct.h"
#include "Arquivos.h"

void ImprimeEstrela(TipoApontadorEstrela arvore) {
    if (arvore == NULL) return;

    if (arvore->Pt == Interna) {
        // Nó interno: intercala recursão nos filhos com impressão das chaves-separadoras
        int i = 0;
        while (i <= arvore->UU.U0.ni) {
            ImprimeEstrela(arvore->UU.U0.pi[i]);          // desce no filho i
            if (i != arvore->UU.U0.ni)
                printf("%d ", arvore->UU.U0.ri[i]); // imprime chave separadora
            i++;
        }
    } else {
        // Nó externo (folha): imprime todos os registros armazenados
        for (int i = 0; i < arvore->UU.U1.ne; i++)
            printf("%d ", arvore->UU.U1.re[i].chave);
    }
    printf("\n");
}

void LiberaArvoreEstrela(TipoApontadorEstrela arvore) {
    if (arvore == NULL)
        return;

    // Só páginas internas possuem filhos
    if (arvore->Pt == Interna) {
        for (int i = 0; i <= arvore->UU.U0.ni; i++) {
            LiberaArvoreEstrela(arvore->UU.U0.pi[i]);
        }
    }

    free(arvore);
}

void PesquisaEstrela(TipoRegistro *x, TipoApontadorEstrela Ap, Bench *bench)
{
    int i;
    TipoApontadorEstrela Pag;
    Pag = Ap;
    
    if (Ap->Pt == Interna) {
        i = 1;
        while (i < Pag->UU.U0.ni && x->chave > Pag->UU.U0.ri[i - 1]){
            i++;
            bench->comp++;
        } 
        bench->comp++;
        bench->comp++;
        if (x->chave < Pag->UU.U0.ri[i - 1])
            PesquisaEstrela(x, Pag->UU.U0.pi[i - 1], bench);
        else PesquisaEstrela(x, Pag->UU.U0.pi[i], bench);
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


void InsereNaPaginaEstrela(TipoApontadorEstrela Ap, TipoRegistro Reg, TipoApontadorEstrela ApDir, Bench *bench){
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

void InsEstrela(TipoRegistro Reg, TipoApontadorEstrela *Ap, bool *Cresceu, TipoRegistro *RegRetorno, TipoApontadorEstrela *ApRetorno, Bench *bench){
    long i =1; long j;
    TipoApontadorEstrela ApTemp;

    if (*Ap == NULL) { // isso so acontece  
        
        *Cresceu = true;

        *RegRetorno = Reg;
        (*ApRetorno) = NULL;
        return;
    }      
           
    if((*Ap)->Pt == Interna){
        while (i < (*Ap)->UU.U0.ni && Reg.chave > (*Ap)->UU.U0.ri[i-1]){
            i++;
            bench->comp++;
        } 
    }
    else {
        while (i < (*Ap)->UU.U1.ne && Reg.chave > (*Ap)->UU.U1.re[i-1].chave){
            i++;
            bench->comp++;
        }    
    }
    if ((*Ap)->Pt == Externa) {
        bench->comp++;
        if (Reg.chave == (*Ap)->UU.U1.re[i-1].chave) {
            printf("Erro: Registro ja existente\n");
            *Cresceu = false;
            return;
        }
        else
            i--;
        
    }
               
    bench->comp++;
    if ((*Ap)->Pt == Interna) {
        if (Reg.chave < (*Ap)->UU.U0.ri[i-1]) i--;
        InsEstrela(Reg, &(*Ap)->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno, bench);
    }
    
    bench->comp++;
    if ((*Ap)->Pt == Interna && !*Cresceu)  
        return;

    if ((*Ap)->Pt == Externa){

        if((*Ap)->UU.U1.ne < 2*M){
            InsereNaPaginaEstrela(*Ap, Reg ,NULL  , bench);
            *Cresceu = false;
            return;
        }
        else{
            ApTemp = (TipoApontadorEstrela) malloc(sizeof(TipoPaginaEstrela));
            ApTemp->Pt = Externa;
            ApTemp->UU.U1.ne = 0;
            if(i < M){
                InsereNaPaginaEstrela(ApTemp, (*Ap)->UU.U1.re[2*M-1] ,NULL , bench);
                (*Ap)->UU.U1.ne--;
                InsereNaPaginaEstrela(*Ap, Reg ,NULL , bench);
            }
            else InsereNaPaginaEstrela(ApTemp, Reg, NULL,bench);
            int k = M;
            for (j = 0; j < M; j++){
                InsereNaPaginaEstrela(ApTemp, (*Ap)->UU.U1.re[k], NULL ,bench);
                k++;
            }
            (*Ap)->UU.U1.ne = M;
            *RegRetorno = (*Ap)->UU.U1.re[M];
            *ApRetorno = ApTemp;
            *Cresceu = true;    
        }
    }
    else{
        bench->comp++;
        if ((*Ap)->UU.U0.ni < 2*M) { //Se houver espaco na pagina, insere o registro e nao precisa dividir
            InsereNaPaginaEstrela(*Ap, *RegRetorno, *ApRetorno, bench);
            *Cresceu = false;
            return;
        }
        //Se a pagina tiver cheia, precisa dividir
        ApTemp = (TipoApontadorEstrela) malloc(sizeof(TipoPaginaEstrela));
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 0;
        ApTemp->UU.U0.pi[0] = NULL;
        TipoRegistro x;
        if (i < M + 1) {//Decide se a chave entra na metade esquerda ou direita
            //Pega o ultimo elemento da pagina, remove da esquerda e insere na direita (pag dos maiores)
             
            x.chave = (*Ap)->UU.U0.ri[2*M-1];
            InsereNaPaginaEstrela(ApTemp, x, (*Ap)->UU.U0.pi[2*M],bench);
            (*Ap)->UU.U0.ni--;
            //Insere a chave nova na pagina da esquerda
            InsereNaPaginaEstrela((*Ap), *RegRetorno, *ApRetorno,bench);
        } 
        else //Caso contrario entra direto na pagina direita
            InsereNaPaginaEstrela(ApTemp, *RegRetorno, *ApRetorno,bench);
        for (j = M + 2; j <= 2*M; j++){//Move os elementos da metade direita
            x.chave = (*Ap)->UU.U0.ri[j-1];
            InsereNaPaginaEstrela(ApTemp, x, (*Ap)->UU.U0.pi[j],bench);
        }
        x.chave = (*Ap)->UU.U0.ri[M];
        (*Ap)->UU.U0.ni = M;
        ApTemp->UU.U0.pi[0] = (*Ap)->UU.U0.pi[M+1];
        *RegRetorno = x; //Promove a chave do meio para a pagina pai
        *ApRetorno = ApTemp;
        *Cresceu = true;
    }       
}

void InsereEstrela(TipoRegistro Reg, TipoApontadorEstrela *Ap, Bench *bench) {
    bool Cresceu = false;
    TipoRegistro RegRetorno;
    TipoApontadorEstrela ApRetorno, ApTemp;
    InsEstrela(Reg, Ap, &Cresceu, &RegRetorno, &ApRetorno,bench);
    
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
            printf("Cresceu externa\n\n");
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

void arvoreBEstrela(int chave, int situacao, Bench *bench, int printFlag, int tam) {
    TipoApontadorEstrela pArvore = NULL;
    FILE* pArq = criaArquivos(situacao, printFlag);
    if (!pArq) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }
    TipoItem temp = {0};
    int i = 0;
    while (i < tam) {
        if (fread(&temp, sizeof(TipoItem), 1, pArq) != 1)
            return;
        InsereEstrela(temp, &pArvore, bench);
        bench->transf++;
        i++;
    }
        
    TipoRegistro busca = {0};
    busca.chave = chave;
    PesquisaEstrela(&busca, pArvore,bench);
    
    printf("ARVORE\n");
    ImprimeEstrela(pArvore);
    LiberaArvoreEstrela(pArvore);
    fclose(pArq);
}