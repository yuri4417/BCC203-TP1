#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "Executavel.h"
#include "ArvoreB.h"
#include "Struct.h"
#include "Arquivos.h"

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
    
    //Procura a chave na pagina
    while (i < Ap->n && x->chave > Ap->r[i-1].chave) {
        i++;
        bench->comp++;
    }
    
    
    if(x->chave == Ap->r[i-1].chave) {//Se achar printa ela
        *x = Ap->r[i-1];
        printItem(x);
        return;
    }
    bench->comp++;
    
    bench->comp++;
    if (x->chave < Ap->r[i-1].chave) //Se for menor ele desce no apontador a esquerda
        Pesquisa(x, Ap->p[i-1], bench);
    else //Se for maior ele desce no apontador a direita
        Pesquisa(x, Ap->p[i], bench);
}

void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir, Bench *bench){
    int k;
    k = Ap->n;
    while (k>0) {
        //Enquanto a chave for menor, ele vai deslocando os elementos para a direita
        bench->comp++;
        if (Reg.chave >= Ap->r[k-1].chave) {
            break;
        }
        Ap->r[k] = Ap->r[k-1];
        Ap->p[k+1] = Ap->p[k];
        k--;
    }
    //Achou aonde colocar a chave, entao insere
    Ap->r[k] = Reg;
    Ap->p[k+1] = ApDir;
    Ap->n++;
}

void Ins(TipoRegistro Reg, TipoApontador Ap, bool *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno, Bench *bench){
    long i =1; long j;
    TipoApontador ApTemp;
    //Se chegar em uma pagina nula, entao a chave deve ser inserida ali, entao retorna a chave para ser inserida na pagina pai
    if (Ap == NULL) {
        *Cresceu = true;
        *RegRetorno = Reg;
        (*ApRetorno) = NULL;
        return;
    }
    //Procura em qual filho descer
    while (i < Ap->n && Reg.chave > Ap->r[i-1].chave){
        i++;
        bench->comp++;
    } 

    bench->comp++;
    if (Reg.chave == Ap->r[i-1].chave) { // se a chave ja existe, nao insere
        printf("Erro: Registro ja existente\n");
        *Cresceu = false;
        return;
    }
    
    bench->comp++;
    if (Reg.chave < Ap->r[i-1].chave) 
        i--;
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno, bench); //Desce para o filho aonde vai inserir

    if (!*Cresceu)//Se nao cresceu ele retorna
        return;

    bench->comp++;
    if (Ap->n < 2*M) { //Se houver espaco na pagina, insere o registro e nao precisa dividir
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno, bench);
        *Cresceu = false;
        return;
    }
    //Se a pagina tiver cheia, precisa dividir
    ApTemp = (TipoApontador) malloc(sizeof(TipoPagina));
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;
    if (i < M + 1) {//Decide se a chave entra na metade esquerda ou direita
        //Pega o ultimo elemento da pagina, remove da esquerda e insere na direita (pag dos maiores)
        InsereNaPagina(ApTemp, Ap->r[2*M-1], Ap->p[2*M],bench);
        Ap->n--;
        //Insere a chave nova na pagina da esquerda
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno,bench);
    } 
    else //Caso contrario entra direto na pagina direita
        InsereNaPagina(ApTemp, *RegRetorno, *ApRetorno,bench);
    for (j = M + 2; j <= 2*M; j++)//Move os elementos da metade direita
        InsereNaPagina(ApTemp, Ap->r[j-1], Ap->p[j],bench);
    Ap->n = M;
    ApTemp->p[0] = Ap->p[M+1];
    *RegRetorno = Ap->r[M]; //Promove a chave do meio para a pagina pai
    *ApRetorno = ApTemp;
    
}

void Insere(TipoRegistro Reg, TipoApontador *Ap, Bench *bench) {
    bool Cresceu;
    TipoRegistro RegRetorno;
    TipoApontador ApRetorno, ApTemp;
    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno,bench);
    //Se a raiz crescer, entao cria uma nova pagina e coloca a chave promovida nela
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

void arvoreB(int chave, int situacao, Bench *bench, int printFlag, int tam) {
    TipoApontador pArvore = NULL;
    FILE* pArq = criaArquivos(situacao, printFlag, tam);
    if (!pArq) {
        printf("Erro ao abrir o arquivo\n");
        return;
    }
    // Timer timer;
    // timerStart(&timer);
    TipoRegistro temp = {0};
    int i = 0;
    while (i < tam) {
        i++;
        fread(&temp, sizeof(TipoRegistro), 1, pArq);
        bench->transf++;
        Insere(temp, &pArvore, bench);
    }
        
    TipoRegistro busca = {0};
    busca.chave = chave;
    Pesquisa(&busca, pArvore,bench);
    // bench->tempoExec = timerStop(&timer);
    LiberaArvore(pArvore);
    fclose(pArq);
}