#include <stdio.h>
#include "ABB.h"
#include "Struct.h"
void insereFilhos(FILE *arq, int chave, int pos, int paiAtual){
    fseek(arq, paiAtual * sizeof(ItemABB), SEEK_SET);
    ItemABB pesq;
    fread(&pesq, sizeof(ItemABB), 1, arq);
    if (chave < pesq.item.chave) {
        if (pesq.esq == -1) {
            pesq.esq = pos;
            fseek(arq, paiAtual * sizeof(ItemABB), SEEK_CUR);
            fwrite(&pesq, sizeof(ItemABB), 1, arq);
            return;
        }
        else 
            insereFilhos(arq, chave, pos, pesq.esq);
    }
    else {
        if (pesq.dir == -1) {
            pesq.dir = pos;// usar o ftell, subtrair pelo sizeof, e voltar com SEEK_set 
            //int pArq = ftell(arq) - sizeof(ItemABB);
            ////fseek(arq, pArq*sizeof(ItemABB, SEEK_SET));
            fseek(arq, paiAtual * sizeof(ItemABB), SEEK_CUR);
            fwrite(&pesq, sizeof(ItemABB), 1, arq);
            return;
        }
        else 
            insereFilhos(arq, chave, pos, pesq.dir); 
    }
}
// PRoblema está aqui !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void criaArquivoABB(int situacao) {
    FILE *arq = fopen("abb.bin", "wb");
    FILE *arqRef;
    if (situacao == 1) arqRef = fopen("arqAscendente.bin", "rb");
    else if (situacao == 2) arqRef = fopen("arqDescendente.bin", "rb");
    else arqRef = fopen("arqAleatorio.bin","rb");
    if (!arq || !arqRef){
        printf("Erro na abertura do arquivo!\n");
        return;
    }
    ItemABB temp;
    while(fread(&temp.item, sizeof(TipoItem), 1, arqRef) == 1){
        temp.esq = temp.dir = -1;
        
        // Colocar no final do abb.bin
        fseek(arq, 0, SEEK_END);
        int pos = ftell(arq) / sizeof(ItemABB);
        fwrite(&temp, sizeof(ItemABB), 1, arq);
        
        fseek(arq, 0, SEEK_SET);
        if(pos > 1)
            insereFilhos(arq, temp.item.chave, pos, 0);
    }

    fclose(arq);
    fclose(arqRef);
}

int pesquisaABB(int chave, int situacao) {
    criaArquivoABB(situacao);
    FILE *arq = fopen("abb.bin", "rb");
    if (!arq)
       return -1;
    
    ItemABB pesq;
    int pos = 0;
    
    while (1) {
        fseek(arq, pos * sizeof(ItemABB), SEEK_SET);
        
        if (fread(&pesq, sizeof(ItemABB), 1, arq) != 1)
            break;
        
        
        if (chave < pesq.item.chave) {
            if (pesq.esq == -1) break;
            pos = pesq.esq;
        }
        else if (pesq.item.chave == chave) {
            printf("Item encontrado: Chave: %d\n", pesq.item.chave);
            fclose(arq);
            return 1;
        }
        else if (chave > pesq.item.chave) {
            if (pesq.dir == -1) break;
            pos = pesq.dir;
        }
    }
    printf("Item nao encontrado.\n");
    fclose(arq);
    return -1;
}
