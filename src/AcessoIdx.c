#include <stdio.h>
#include <time.h>
#include "AcessoIdx.h"
#include "Executavel.h"
#include "Arquivos.h"

int comparaCrescente(int valor, int chave) {
    return valor < chave;
}

int comparaDecrescente(int valor, int chave) {
    return valor > chave;
}

int pesquisaBinaria(TipoItem *v, int esq, int dir, int chave, int (*compara)(int, int), Bench *bench) {
    if (esq > dir)
        return -1;

    int m = (esq + dir) / 2;
    bench->comp++;
    if (v[m].chave == chave)
        return m;
    bench->comp++;
    if (compara(v[m].chave, chave)) {
        return pesquisaBinaria(v, m + 1, dir, chave,compara, bench);
    }
    else
        return pesquisaBinaria(v, esq, m - 1, chave,compara, bench);

}


int acessoIndexado(TipoIndice tabela[],TipoItem *item, int situacao, Bench *bench, int tam, int printFlag) {
    TipoItem pagina[ITENSPAGINA];
    int i, quantitens;
    long desloc;
    i = 0; 
    FILE *pArq =  criaArquivos(situacao, printFlag);
    
    if (!pArq) {
        printf("Erro ao abrir o pArquivo.\n");
        return -1;
    }

    int pos = 0;
    int chaveBusca = item->chave;


    TipoItem temp;
    while (pos < tam) { 
        fread(&temp, sizeof(TipoItem), 1, pArq);
        bench->transf++;
        tabela[pos].chave = temp.chave;
        tabela[pos].posicao = pos+1;
        pos++;
        fseek(pArq, sizeof(TipoItem) * (ITENSPAGINA-1), SEEK_CUR);
    }
    
    int crescente = (tabela[0].chave < tabela[1].chave);// 1 para crescente, 0 para decrescente
    bench->comp++;
    if(crescente){
        while (i < pos && tabela[i].chave <= chaveBusca){
            i++;
            bench->comp++;
        } // <= para crescente, >= para decrescente,
        
    }
    else{
        while (i < pos && tabela[i].chave >= chaveBusca){
            i++;
            bench->comp++;
        }
    }
    if (i == 0){

        fclose(pArq);
        return 0;

    }
    
    else {
        if (i < pos)       
            quantitens = ITENSPAGINA;      //ve se a pagina ta completa
        else {
            fseek (pArq, 0, SEEK_END);
            quantitens = (ftell(pArq)/sizeof(TipoItem))%ITENSPAGINA;
            if (!quantitens) 
                quantitens = ITENSPAGINA;  
        }
        desloc = (tabela[i-1].posicao-1)*ITENSPAGINA*sizeof(TipoItem);

        fseek (pArq, desloc, SEEK_SET);
        fread (&pagina, sizeof(TipoItem), quantitens, pArq);
        bench->transf++;
        
        if(crescente)
            i = pesquisaBinaria(pagina, 0, quantitens-1, chaveBusca, comparaCrescente, bench);
        else
            i = pesquisaBinaria(pagina, 0, quantitens-1, chaveBusca, comparaDecrescente, bench);
        
        if(i >= 0) {
            *item = pagina[i];
            printItem(item);
            fclose (pArq);
            return 1;
        }
        else {
            printf("Item nao encontrado.\n");
            fclose (pArq);
            return 0;
        }       
    }
} 