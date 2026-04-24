#include <stdio.h>
#include <time.h>
#include "AcessoIdx.h"


int pesquisaBinaria(TipoItem *v, int esq, int dir, int chave) {
    if (esq > dir)
        return -1;

    int m = (esq + dir) / 2;

    if (v[m].chave == chave)
        return m;

    else if (v[m].chave < chave)
        return pesquisaBinaria(v, m + 1, dir, chave);
    else
        return pesquisaBinaria(v, esq, m - 1, chave);
}


int acessoIndexado(TipoIndice tabela[], TipoItem* item) {
    TipoItem pagina[ITENSPAGINA];
    int i, quantitens;
    long desloc;
    // Procura pela pagina onde o item pode se encontrar
    i = 0; 

    FILE *arq; 
    if(!(arq = fopen("arqAscendente.bin", "rb"))) {
        printf("Erro ao abrir o arquivo.\n");
        return -1;
    }

    // Leitura das primeiras posições de cada pagina
    int pos = 0;
    int chaveBusca = item->chave;
    TipoItem temp;
    while (fread(&temp, sizeof(TipoItem), 1, arq) == 1) { 
        tabela[pos].chave = temp.chave;
        tabela[pos].posicao = pos+1;
        pos++;
        fseek(arq, sizeof(TipoItem) * 3, SEEK_CUR);
    }

    while (i < pos && tabela[i].chave <= chaveBusca) 
        i++;
    // Caso a chave desejada seja menor que a 1 chave, o item nao existe no arquivo
    if (i == 0) 
        return 0;
    else {
        // A ultima pagina pode nao estar completa
        if (i < pos)
            quantitens = ITENSPAGINA;
        else {
            fseek (arq, 0, SEEK_END);
            quantitens = (ftell(arq)/sizeof(TipoItem))%ITENSPAGINA;
            if (!quantitens) 
                quantitens = ITENSPAGINA; // caso o modulo for igual a ele nao iria rodar o loop, entao atualizar o valor 
        }
        // Le a pagina desejada do arquivo
        desloc = (tabela[i-1].posicao-1)*ITENSPAGINA*sizeof(TipoItem);

        fseek (arq, desloc, SEEK_SET);
        fread (&pagina, sizeof(TipoItem), quantitens, arq);
        // Pesquisa sequencial na pagina lida
        i = pesquisaBinaria(pagina, 0, quantitens-1, chaveBusca);
        
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