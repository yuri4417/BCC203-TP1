#include <stdio.h>
#include "ABB.h"
#include "Struct.h"

void insereFilhos(FILE *arq, int chave, int pos) {
    ItemABB atual;
    int indiceAtual = 0;
    while (1) {
        // move o ponteiro do  arquivo para a leitura
        fseek(arq, indiceAtual * sizeof(ItemABB), SEEK_SET);
        if (fread(&atual, sizeof(ItemABB), 1, arq) != 1) {
            return; // Erro de leitura
        }
        // Verifica se vai para a esquerda ou direita
        if (chave < atual.item.chave) {
            // se nao tiver filhos e so inserir
            if (atual.esq == -1) {
                atual.esq = pos;
                fseek(arq, indiceAtual * sizeof(ItemABB), SEEK_SET);
                fwrite(&atual, sizeof(ItemABB), 1, arq);
                return; 
            } else // se tiver filhos atualiza o endereco
                indiceAtual = atual.esq;
        } 
        else {
            // se nao tiver filhos e so inserir
            if (atual.dir == -1) {
                atual.dir = pos;
                fseek(arq, indiceAtual * sizeof(ItemABB), SEEK_SET);
                fwrite(&atual, sizeof(ItemABB), 1, arq);
                return;
            } else {
                // se tiver filhos atualiza o endereco
                indiceAtual = atual.dir;
            }
        }
    }
}

void criaArquivoABB(int situacao) {
    FILE *arq = fopen("abb.bin", "w+b");
    FILE *arqRef;
    if (situacao == 1) arqRef = fopen("arqAscendente.bin", "rb");
    else if (situacao == 2) arqRef = fopen("arqDescendente.bin", "rb");
    else arqRef = fopen("arqAleatorio.bin","rb");
    if (!arq || !arqRef){
        printf("Erro na abertura do arquivo!\n");
        return;
    }
    ItemABB temp;
    int pos = 0;
    // Lê da referencia
    while(fread(&temp.item, sizeof(TipoItem), 1, arqRef) == 1){
        temp.esq = temp.dir = -1;
        
        // Colocar no final do abb.bin
        fseek(arq, 0, SEEK_END);
        fwrite(&temp, sizeof(ItemABB), 1, arq);
        
        if(pos > 0)
            insereFilhos(arq, temp.item.chave, pos);
        pos++;
    }

    fclose(arq);
    fclose(arqRef);
}

int pesquisaABB(int chave, int situacao) {
    criaArquivoABB(situacao);

    FILE *arq = fopen("abb.bin", "rb");
    if (!arq)
    {
        printf("Erro ao abrir o arquivo!\n");
        return -1;
    }
    ItemABB pesq;
    int pos = 0;
    
    while (1) {
        fseek(arq, pos * sizeof(ItemABB), SEEK_SET);

        if (fread(&pesq, sizeof(ItemABB), 1, arq) != 1)
        {
            fclose(arq);
            break;
        }

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
