#include <stdio.h>
#include <time.h>
#include "AcessoIdx.h"
#include "Executavel.h"
#include "Arquivos.h"
#include <math.h>

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
    FILE *pArq =  criaArquivos(situacao, printFlag, tam);
    // Timer timer;
    // timerStart(&timer);
    if (!pArq) {
        printf("Erro ao abrir o pArquivo.\n");
        return -1;
    }

    int pos = 0;
    TipoItem temp;
    int maxPags = ceil((double)tam / ITENSPAGINA);
    
    while (pos < maxPags)  { 
        
        if (fread(&temp, sizeof(TipoItem), 1, pArq) != 1)
            break;
        bench->transf++;
        
        tabela[pos].chave = temp.chave;
        tabela[pos].posicao = pos+1;
        pos++; 
        if(pos< maxPags)
            fseek(pArq, sizeof(TipoItem) * (ITENSPAGINA-1), SEEK_CUR);
    
    }

    int chaveBusca = item->chave;
    int quantitens;
    TipoItem pagina[ITENSPAGINA];
    int crescente;
    if(situacao == 1)
        crescente = 1;
    else if(situacao == 2)
        crescente = 0;
    int i = 0;
    if(crescente){
        while (i < pos && tabela[i].chave <= chaveBusca){
            i++;
            bench->comp++;
        } // <= para crescente, >= para decrescente,
    }
    else{
        bench->comp++;
        while (i < pos && tabela[i].chave >= chaveBusca){
            i++;
            bench->comp++;
        }
        bench->comp++;
    }
    if (i == 0){  //se ela é menor que a primeira nao tem
        fclose(pArq);  
        return 0;
    }    
    else {
        if (i < pos)
            quantitens = ITENSPAGINA;
        else {
            quantitens = tam % ITENSPAGINA;
            if (quantitens == 0) 
                quantitens = ITENSPAGINA;  
        }
        
        long desloc = (tabela[i-1].posicao-1)*ITENSPAGINA*sizeof(TipoItem);

        fseek (pArq, desloc, SEEK_SET);
        fread (&pagina, sizeof(TipoItem), quantitens, pArq);
        bench->transf++;
        int ind;
        if(crescente)
            ind = pesquisaBinaria(pagina, 0, quantitens-1, chaveBusca, comparaCrescente, bench);
        else
            ind = pesquisaBinaria(pagina, 0, quantitens-1, chaveBusca, comparaDecrescente, bench);
        
        if(ind >= 0) {
            *item = pagina[ind];  
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
    // bench->tempoExec = timerStop(&timer);
}