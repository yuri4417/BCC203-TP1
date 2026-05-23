#include <stdio.h>
#include <time.h>
#include "AcessoIdx.h"
#include "Executavel.h"
#include "Arquivos.h"
#include <stdlib.h>
#include <math.h>

//Se for crescente compara dois valores
int comparaCrescente(int valor, int chave) {
    return valor < chave;
}

//Se for descrescente compara dois valores
int comparaDecrescente(int valor, int chave) {
    return valor > chave;
}

//Faz a Pesquisa Binaria quando chegar na pagina correta
int pesquisaBinaria(TipoItem *v, int esq, int dir, int chave, int (*compara)(int, int), Bench *bench) {
    //Se esq passar da dir quer dizer que o item nao existe
    if (esq > dir)
        return -1;

    //Calcula o meio
    int m = (esq + dir) / 2;

    bench->comp++;
    //Se a chave for igual retorna a pos da chave que achou 
    if (v[m].chave == chave)
        return m;
    

    bench->comp++;
    //Compara as chaves para crescente ou decrescente
    if (compara(v[m].chave, chave)) {
        //Busca metade direita
        return pesquisaBinaria(v, m + 1, dir, chave,compara, bench);
    }
    else
        //Busca metade esquerda
        return pesquisaBinaria(v, esq, m - 1, chave,compara, bench);
}

//Faz a busca por Acesso Indexado
int acessoIndexado(int chave, int situacao, Bench *bench, int tam, FILE* pArq) {
    //Cria a tabela de indices
    TipoIndice *tabela = malloc(sizeof(TipoIndice)*(ceil(tam/ITENSPAGINA)));
    if(!tabela)
    {
        printf("Erro ao alocar memoria.\n");
        exit(1);
    }
    
    //deslocFile serve para arquivos decrescentes
    long deslocFile = 0;
    if (situacao == ARQDESC) {   
        //Desloca o ponteiro para manter as chaves de tam a 1 no arquivo 
        deslocFile = sizeof(TipoItem) * (MAXTAM - tam);
        fseek(pArq, deslocFile, SEEK_SET);
    }

    //Posicao atual da tabela indice
    int pos = 0;
    TipoItem temp;

    //Quantidade total de paginas
    int maxPags = ceil((double)tam / ITENSPAGINA);

    //Faz a tabela de indices
    while (pos < maxPags)  { 
        //Le o primeiro item da pag
        if (fread(&temp, sizeof(TipoItem), 1, pArq) != 1) 
            break;
        bench->transf++;
        tabela[pos].chave = temp.chave; //Guarda a primeira chave da pagina
        tabela[pos].posicao = pos+1; //Guarda numero da pagina
        pos++; 
        //Pula para prox pagina
        fseek(pArq, sizeof(TipoItem) * (ITENSPAGINA-1), SEEK_CUR);
    }

    int chaveBusca = chave;
    int quantitens;
    TipoItem pagina[ITENSPAGINA];

    //Define qual ordenacao vai usar, crescente ou decrescente
    int crescente;
    if(situacao == 1)
        crescente = 1;
    else if(situacao == 2)
        crescente = 0;

    //Busca sequencial na tabela indice para descobrir qual pagina pode conter a chave
    int i = 0;
    if(crescente){
        while (i < pos && tabela[i].chave <= chaveBusca){
            i++;
            bench->comp++;
        } // <= para crescente
        bench->comp++;
    }
    else {
        bench->comp++;
        while (i < pos && tabela[i].chave >= chaveBusca){
            i++;
            bench->comp++;
        }// >= para decrescente,
        bench->comp++;
    }
    //Se i = 0 a chave nao pertence a nenhuma pagina
    if (i == 0) { 
        printf("Item %d nao encontrado!\n",chave);
        fclose(pArq);  
        return 0;
    }    
    else {
        //Se nao for a ultima pagina ela ta cheia
        if (i < pos)
            quantitens = ITENSPAGINA;
        else {
            //Calcula a quantidade rela de itens na ultima paginas
            quantitens = tam % ITENSPAGINA;
            if (quantitens == 0) 
                quantitens = ITENSPAGINA;  
        }
        
        //Calcula o deslocamento da pagina correta
        long desloc = deslocFile + (tabela[i-1].posicao-1) * ITENSPAGINA * sizeof(TipoItem);

        //Vai para a pagina e le ela
        fseek (pArq, desloc, SEEK_SET);
        fread (pagina, sizeof(TipoItem), quantitens, pArq);
        bench->transf++;

        //Faz busca binaria dentro da pagina
        int ind;
        if(crescente)
            ind = pesquisaBinaria(pagina, 0, quantitens-1, chaveBusca, comparaCrescente, bench);
        else
            ind = pesquisaBinaria(pagina, 0, quantitens-1, chaveBusca, comparaDecrescente, bench);
        
        //Se encontrou printa o item
        if(ind >= 0) {
            printItem(&pagina[ind]);
            fclose (pArq);
            free(tabela);
            return 1;
        }
        //Se nao encontrou printa nao encontrado
        else {
            printf("Item %d nao encontrado!\n",chave);
            fclose (pArq);
            free(tabela);
            return 0;
        }       
    }
}
