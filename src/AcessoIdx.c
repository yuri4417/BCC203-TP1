#include <stdio.h>
#include <time.h>
#include "AcessoIdx.h"

int comparaCrescente(int valor, int chave) {
    return valor < chave;
}

int comparaDecrescente(int valor, int chave) {
    return valor > chave;
}

int pesquisaBinaria(TipoItem *v, int esq, int dir, int chave, int (*compara)(int, int)) {
    if (esq > dir)
        return -1;

    int m = (esq + dir) / 2;

    if (v[m].chave == chave)
        return m;

    if (compara(v[m].chave, chave)) {
        return pesquisaBinaria(v, m + 1, dir, chave,compara);
    }
    else
        return pesquisaBinaria(v, esq, m - 1, chave,compara);

}


int acessoIndexado(TipoIndice tabela[], TipoItem* item, int situacao) {
    TipoItem pagina[ITENSPAGINA];
    int i, quantitens;
    long desloc;
    i = 0; 
    FILE *arq = (situacao == 1) ? fopen("arqAscendente.bin", "rb") : fopen("arqDescendente.bin", "rb");
    
    if (!arq) {
        printf("Erro ao abrir o arquivo.\n");
        return -1;
    }

    int pos = 0;
    int chaveBusca = item->chave;
    TipoItem temp;
    while (fread(&temp, sizeof(TipoItem), 1, arq) == 1) { 
        tabela[pos].chave = temp.chave;
        tabela[pos].posicao = pos+1;
        pos++;
        fseek(arq, sizeof(TipoItem) * (ITENSPAGINA-1), SEEK_CUR);
    }
    
    int crescente = (tabela[0].chave < tabela[1].chave);// 1 para crescente, 0 para decrescente
    if(crescente)
        while (i < pos && tabela[i].chave <= chaveBusca) i++; // <= para crescente, >= para decrescente,
    else
        while (i < pos && tabela[i].chave >= chaveBusca) i++;

    if (i == 0){
        fclose(arq);
        return 0;
    }
        
    else {
        if (i < pos)
            quantitens = ITENSPAGINA;
        else {
            fseek (arq, 0, SEEK_END);
            quantitens = (ftell(arq)/sizeof(TipoItem))%ITENSPAGINA;
            if (!quantitens) 
                quantitens = ITENSPAGINA;  
        }
        desloc = (tabela[i-1].posicao-1)*ITENSPAGINA*sizeof(TipoItem);

        fseek (arq, desloc, SEEK_SET);
        fread (&pagina, sizeof(TipoItem), quantitens, arq);

        
        if(crescente)
            i = pesquisaBinaria(pagina, 0, quantitens-1, chaveBusca, comparaCrescente);
        else
            i = pesquisaBinaria(pagina, 0, quantitens-1, chaveBusca, comparaDecrescente);
        
        if(i >= 0) {
            *item = pagina[i];
            printf("Item encontrado: Chave: %d\n", item->chave);
            fclose (arq);
            return 1;
        }
        else {
            printf("Item nao encontrado.\n");
            fclose (arq);
            return 0;
        }
            
    }
    
} 