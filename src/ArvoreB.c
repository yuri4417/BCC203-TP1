#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "Executavel.h"
#include "ArvoreB.h"
#include "Struct.h"
#include "Arquivos.h"

//Função que libera Alocação da Árvore
void LiberaArvore(TipoApontador arvore) {
    if (arvore == NULL)
        return;
    for (int i = 0; i <= arvore->n; i++) {
        LiberaArvore(arvore->p[i]);
    }
    free(arvore);
}

//Função que realiza a pesquisa na Árvore B
void Pesquisa(TipoRegistro *x, TipoApontador Ap, Bench *bench){
    long i = 1;

    // Se apontador apontar pra Nulo
    if (Ap == NULL) {
        printf("\nItem %d nao encontrado!\n",x->chave);
        return;
    } 
    
    //Procura a chave na pagina
    while (i < Ap->n && x->chave > Ap->r[i-1].chave) {
        i++;
        bench->comp++;
    }
    
    // Se encontrar a chave
    if(x->chave == Ap->r[i-1].chave) {
        *x = Ap->r[i-1];
        printItem(x); //Imprime ela
        return;
    }
    bench->comp++;
    

    bench->comp++;
    //Se for menor ele desce no apontador a esquerda
    if (x->chave < Ap->r[i-1].chave)
        Pesquisa(x, Ap->p[i-1], bench);

    //Se for maior ele desce no apontador a direita
    else 
        Pesquisa(x, Ap->p[i], bench);
}


// Função responsável por inserir o item no registro/página
void InsereNaPagina(TipoApontador Ap, TipoRegistro Reg, TipoApontador ApDir, Bench *bench){
    int k;
    k = Ap->n;

    //Enquanto a chave for menor, ele vai deslocando os elementos para a direita
    while (k>0) {
        bench->comp++;
        if (Reg.chave >= Ap->r[k-1].chave) {
            break;
        }
        Ap->r[k] = Ap->r[k-1];
        Ap->p[k+1] = Ap->p[k];
        k--;
    }
    //Encontrou aonde colocar a chave, inserindo
    Ap->r[k] = Reg;
    Ap->p[k+1] = ApDir;
    Ap->n++;
}

//Função de busca do local correto para inserir o registro, além de caminhar pela árvore
void Ins(TipoRegistro Reg, TipoApontador Ap, bool *Cresceu, TipoRegistro *RegRetorno, TipoApontador *ApRetorno, Bench *bench,bool *memCheia){
    long i =1; long j;
    TipoApontador ApTemp;

    //Se chegar em uma pagina nula, entao a chave deve ser inserida ali, retornando a chave para ser inserida na pagina pai
    if (Ap == NULL) {
        *Cresceu = true;
        *RegRetorno = Reg;
        (*ApRetorno) = NULL;
        return;
    }

    //Procura de qual filho descer
    while (i < Ap->n && Reg.chave > Ap->r[i-1].chave){
        i++;
        bench->comp++;
    } 

    //Se a chave existir, nao deve inserir
    bench->comp++;
    if (Reg.chave == Ap->r[i-1].chave) { 
        printf("Erro: Registro %d ja existente\n",Reg.chave);
        *Cresceu = false;
        return;
    }
    

    bench->comp++;
    if (Reg.chave < Ap->r[i-1].chave) 
        i--;

    //Desce para o filho aonde vai inserir
    Ins(Reg, Ap->p[i], Cresceu, RegRetorno, ApRetorno, bench,memCheia); 

    if (!*Cresceu)//Se nao cresceu ele retorna
        return;

    //Se houver espaco na pagina, insere o registro e nao precisa dividir
    bench->comp++;
    if (Ap->n < 2*M) {
        InsereNaPagina(Ap, *RegRetorno, *ApRetorno, bench);
        *Cresceu = false;
        return;
    }

    //Se a pagina tiver cheia, precisa dividir
    ApTemp = (TipoApontador) malloc(sizeof(TipoPagina));
    if(!ApTemp) {
        printf("Erro ao alocar memoria\n");     
        *memCheia = true;
        return;
    }
    ApTemp->n = 0;
    ApTemp->p[0] = NULL;

    //Decide se a chave entra na metade esquerda ou direita
    if (i < M + 1) {
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

//Função responsável por chama função Ins e atribui novos valores dos apontadores
void Insere(TipoRegistro Reg, TipoApontador *Ap, Bench *bench,bool *memCheia) {
    bool Cresceu;
    TipoRegistro RegRetorno;
    TipoApontador ApRetorno, ApTemp;
    Ins(Reg, *Ap, &Cresceu, &RegRetorno, &ApRetorno,bench, memCheia);
    //Se a raiz crescer, entao cria uma nova pagina e coloca a chave promovida nela
    if(*memCheia) {
        return;
    }
    if (Cresceu) {
        ApTemp = (TipoPagina *) malloc(sizeof(TipoPagina));
        if(!ApTemp) {
            printf("Erro ao alocar memoria\n");
            *memCheia = true;     
            return;
        }
        ApTemp->n = 1;
        ApTemp->r[0] = RegRetorno;
        ApTemp->p[1] = ApRetorno;
        ApTemp->p[0] = *Ap;
        *Ap = ApTemp;

    }   
}

// A função da própria arvore B, inicializando, tentando abrir o arquivo e iniciando a leitura do arquivo de registros, depois realizando a pesquisa
void arvoreB(int chave, Bench *bench, int tam, FILE* pArq) {
    TipoApontador pArvore = NULL;
    TipoRegistro temp = {0};
    int i = 0;
    bool memCheia = false;
    //Leitura dos itens do Arquivo de Registro
    while (i < tam) {
        i++;
        fread(&temp, sizeof(TipoRegistro), 1, pArq);
        bench->transf++;
        Insere(temp, &pArvore, bench, &memCheia);
        if(memCheia) {
            printf("Memoria cheia, nao foi possivel inserir todos os registros\n");
            LiberaArvore(pArvore);
            fclose(pArq);
            return;
        }
    }
   
    TipoRegistro busca = {0};
    busca.chave = chave;
    Pesquisa(&busca, pArvore,bench);
    LiberaArvore(pArvore);
    fclose(pArq);
}