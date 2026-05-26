#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "Executavel.h"
#include "ArvoreBEstrela.h"
#include "Struct.h"
#include "Arquivos.h"

//Funcao responsavel pela impressao da arvore B estrela
void ImprimeEstrela(TipoApontadorEstrela arvore) {
    if (arvore == NULL) 
        return;

    // No interno: intercala recursao nos filhos com impressao das chaves-separadoras
    if (arvore->Pt == Interna) {
        int i = 0;
        while (i <= arvore->UU.U0.ni) {
            ImprimeEstrela(arvore->UU.U0.pi[i]);// Desce no filho i
            if (i != arvore->UU.U0.ni)
                printf("%d ", arvore->UU.U0.ri[i]);// Imprime chave separadora
            i++;
        }
    } 

    // No externo (folha): imprime todos os registros armazenados
    else {
        for (int i = 0; i < arvore->UU.U1.ne; i++)
            printf("%d ", arvore->UU.U1.re[i].chave);
    }
    printf("\n");
}

//Funcao responsavel pela desalocacao de memoria da arvore
void liberaArvoreEstrela(TipoApontadorEstrela arvore) {
    if (arvore == NULL)
        return;

    // So paginas internas possuem filhos
    if (arvore->Pt == Interna) {
        for (int i = 0; i <= arvore->UU.U0.ni; i++) {
            liberaArvoreEstrela(arvore->UU.U0.pi[i]);
        }
    }

    free(arvore);
}

//Funcao voltada a realizar a pesquisa na Arvore B*
void PesquisaEstrela(TipoRegistro *x, TipoApontadorEstrela Ap, Bench *bench)
{
    int i;
    TipoApontadorEstrela Pag;
    Pag = Ap;
    
    //Navegacao nos nos internos
    if (Ap->Pt == Interna) {
        i = 1;


        //While para quando encontra uma chave maior ou igual a buscada ou quando acaba as chaves
        while (i < Pag->UU.U0.ni && x->chave > Pag->UU.U0.ri[i - 1]){
            i++;
            bench->comp++;
        } 
        bench->comp++;

        //Escolhe qual filho ira seguir para descer na arvore
        bench->comp++;
        if (x->chave < Pag->UU.U0.ri[i - 1]) // Esquerda
            PesquisaEstrela(x, Pag->UU.U0.pi[i - 1], bench);
        else // Direita
            PesquisaEstrela(x, Pag->UU.U0.pi[i], bench);
        return;
    }

    //Navegacao nos nos externos
    i = 1;

    //Percorre a pagina folha
    while (i < Pag->UU.U1.ne && x->chave > Pag->UU.U1.re[i - 1].chave){
        i++;
        bench->comp++;
    }
    bench->comp++;

    // Verifica se a chave eh a procurada
    bench->comp++;
    if (x->chave == Pag->UU.U1.re[i - 1].chave){
        *x = Pag->UU.U1.re[i - 1];
        printItem(x);
    }
    else 
        printf("Item %d nao encontrado!\n",x->chave);
}

//Funcao que insere o item no registro
void InsereNaPaginaEstrela(TipoApontadorEstrela Ap, TipoRegistro Reg, TipoApontadorEstrela ApDir, Bench *bench){
    int k;

    // Insercao no no interno
    if(Ap->Pt == Interna){
        k = Ap->UU.U0.ni;
        while(k>0){
            bench->comp++;

            // Verifica se acho a posicao correta
            if (Reg.chave >= Ap->UU.U0.ri[k-1])
                break;

            // Se nao, passa pra direita
            Ap->UU.U0.ri[k] = Ap->UU.U0.ri[k-1];
            Ap->UU.U0.pi[k+1] = Ap->UU.U0.pi[k];
            k--;
        }

        Ap->UU.U0.ri[k] = Reg.chave;
        Ap->UU.U0.pi[k+1] = ApDir;
        Ap->UU.U0.ni++;
    }    

    // Insercao no no externo
    else{
        k = Ap->UU.U1.ne;
      
        while(k>0){
            bench->comp++;

            // Verifica se acho a posicao correta
            if (Reg.chave >= Ap->UU.U1.re[k-1].chave)
                break;
            // Se nao, passa pra direita
            Ap->UU.U1.re[k] = Ap->UU.U1.re[k-1];
            k--;
        }

        Ap->UU.U1.re[k] = Reg;
        Ap->UU.U1.ne++;
    }

}

//Funcao responsavel por realizar a busca de qual e a posicao certa para inserir o registro
void InsEstrela(TipoRegistro Reg, TipoApontadorEstrela *Ap, bool *Cresceu, TipoRegistro *RegRetorno, TipoApontadorEstrela *ApRetorno, Bench *bench, bool *memCheia){
    long i =1; long j;
    TipoApontadorEstrela ApTemp;

    
    if (*Ap == NULL) { // Arvore Vazia  
        *Cresceu = true;
        *RegRetorno = Reg;
        (*ApRetorno) = NULL;
        return;
    }      

    // Andar pela arvore
    // Se for no interno...
    if((*Ap)->Pt == Interna){
        while (i < (*Ap)->UU.U0.ni && Reg.chave > (*Ap)->UU.U0.ri[i-1]){
            i++;
            bench->comp++;
        } 
    }

    // Se for no externo...
    else {
        while (i < (*Ap)->UU.U1.ne && Reg.chave > (*Ap)->UU.U1.re[i-1].chave){
            i++;
            bench->comp++;
        }    
    }

    // Nos folhas
    if ((*Ap)->Pt == Externa) {
        bench->comp++;

        // N deixa inserir item que ja existe
        if (Reg.chave == (*Ap)->UU.U1.re[i-1].chave) {
            printf("Erro: Registro ja existente\n");
            *Cresceu = false;
            return;
        }
        else
            i--;
    }
               
    // Nos internos
    bench->comp++;
    if ((*Ap)->Pt == Interna) {

        // Comparacao para saber se vai pela esquerda ou direita
        if (Reg.chave < (*Ap)->UU.U0.ri[i-1]) 
            i--;

        // Vai para o filho
        InsEstrela(Reg, &(*Ap)->UU.U0.pi[i], Cresceu, RegRetorno, ApRetorno, bench, memCheia);
    }
    
    bench->comp++;

    if ((*Ap)->Pt == Interna && !*Cresceu)  
        return;

    // Insercao em no folha
    if ((*Ap)->Pt == Externa){

        // Tem espaço...
        if((*Ap)->UU.U1.ne < 2*M){
            InsereNaPaginaEstrela(*Ap, Reg ,NULL  , bench);
            *Cresceu = false;
            return;
        }

        //Folha cheia...
        else{
            ApTemp = (TipoApontadorEstrela) malloc(sizeof(TipoPaginaEstrela));
            if(!ApTemp) {
                printf("Erro ao alocar memoria\n");  
                *memCheia = true;   
                return;
            }

            // Nova pagina = Folha
            ApTemp->Pt = Externa;
            ApTemp->UU.U1.ne = 0;

            // Registro na esquerda
            if(i < M){
                InsereNaPaginaEstrela(ApTemp, (*Ap)->UU.U1.re[2*M-1] ,NULL , bench);
                (*Ap)->UU.U1.ne--;
                InsereNaPaginaEstrela(*Ap, Reg ,NULL , bench);
            }

            // Registro na direita
            else 
                InsereNaPaginaEstrela(ApTemp, Reg, NULL,bench);
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

    // Insercao em no interno/índice
    else{
        bench->comp++;
        if ((*Ap)->UU.U0.ni < 2*M) { // Se houver espaco na pagina, insere o registro e nao precisa dividir
            InsereNaPaginaEstrela(*Ap, *RegRetorno, *ApRetorno, bench);
            *Cresceu = false;
            return;
        }
        // Se a pagina tiver cheia, precisa dividir
        ApTemp = (TipoApontadorEstrela) malloc(sizeof(TipoPaginaEstrela));
        if(!ApTemp) {
            printf("Erro ao alocar memoria\n");  
            *memCheia = true;   
            return;
        }
        ApTemp->Pt = Interna;
        ApTemp->UU.U0.ni = 0;
        ApTemp->UU.U0.pi[0] = NULL;
        TipoRegistro x; //porcausa da insere na pag
        if (i < M + 1) {// Decide se a chave entra na metade esquerda ou direita

            // Pega o ultimo elemento da pagina, remove da esquerda e insere na direita (pag dos maiores)
            x.chave = (*Ap)->UU.U0.ri[2*M-1];
            InsereNaPaginaEstrela(ApTemp, x, (*Ap)->UU.U0.pi[2*M],bench);
            (*Ap)->UU.U0.ni--;
            // Insere a chave nova na pagina da esquerda
            InsereNaPaginaEstrela((*Ap), *RegRetorno, *ApRetorno,bench);
        } 
        else // Caso contrario entra direto na pagina direita
            InsereNaPaginaEstrela(ApTemp, *RegRetorno, *ApRetorno,bench);
        for (j = M + 2; j <= 2*M; j++){// Move os elementos da metade direita
            x.chave = (*Ap)->UU.U0.ri[j-1];
            InsereNaPaginaEstrela(ApTemp, x, (*Ap)->UU.U0.pi[j],bench);
        }
        x.chave = (*Ap)->UU.U0.ri[M];
        (*Ap)->UU.U0.ni = M;
        ApTemp->UU.U0.pi[0] = (*Ap)->UU.U0.pi[M+1];
        *RegRetorno = x; // Promove a chave do meio para a pagina pai
        *ApRetorno = ApTemp;
        *Cresceu = true;
    }       
}

// Funcao de insercao chama InsEstrela e, caso necessario, cria nova pagina raiz
void InsereEstrela(TipoRegistro Reg, TipoApontadorEstrela *Ap, Bench *bench, bool *memCheia){ 
    bool Cresceu = false;
    TipoRegistro RegRetorno;
    TipoApontadorEstrela ApRetorno, ApTemp;
    InsEstrela(Reg, Ap, &Cresceu, &RegRetorno, &ApRetorno,bench, memCheia);
    
    if(*memCheia) 
        return;

    if (Cresceu) {        
        // Aloca nova pagina de no raiz
        ApTemp = (TipoPaginaEstrela *) malloc(sizeof(TipoPaginaEstrela));
        if(!ApTemp) {
            printf("Erro ao alocar memoria\n");     
            *memCheia = true;
            return;
        }

        // Arvore vazia
        if(*Ap == NULL){
            ApTemp->Pt = Externa;
            ApTemp->UU.U1.ne = 1;
            ApTemp->UU.U1.re[0] = RegRetorno;
            *Ap = ApTemp;
        }

        // Arvore nao vazia, nova raiz vira no interno
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

// Funcao do método, chamando todas as funções complementares, abrindo o arquivo e lendo os valores do arquivo de registros
void arvoreBEstrela(int chave, Bench *bench, int tam, FILE *pArq) {
    TipoApontadorEstrela pArvore = NULL;

    bool memCheia = false;
    
    TipoRegistro temp = {0};
    int i = 0;
    while (i < tam) {
        if (fread(&temp, sizeof(TipoRegistro), 1, pArq) != 1)
            return;
        InsereEstrela(temp, &pArvore, bench, &memCheia);
        if(memCheia) {
            printf("Memoria cheia, nao foi possivel inserir todos os elementos\n");
            liberaArvoreEstrela(pArvore);
            fclose(pArq);
            return;
        }
        bench->transf++;
        i++;
    }
        
    TipoRegistro busca = {0};
    busca.chave = chave;
    PesquisaEstrela(&busca, pArvore,bench);
    
    // ImprimeEstrela(pArvore);
    liberaArvoreEstrela(pArvore);
    fclose(pArq);
}