#include <stdio.h>
#include "ABB.h"
#include "Struct.h"
#include "Executavel.h"
#include "Arquivos.h"
void insereFilhos(FILE *arq, int chave, int pos, Bench *bench) {
    ItemABB atual;
    int indiceAtual = 0;
    while (1) {
        // move o ponteiro do  arquivo para a leitura
        fseek(arq, indiceAtual * sizeof(ItemABB), SEEK_SET);
        bench->transf++;
        if (fread(&atual, sizeof(ItemABB), 1, arq) != 1) {
            return; // Erro de leitura
        }   
        // Verifica se vai para a esquerda ou direita
        bench->comp++;
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

void criaArquivoABB(int situacao, Bench *bench, int printFlag, int tam) {
    FILE *arq = fopen("abb.bin", "w+b");
    FILE *pArqRef = criaArquivos(situacao, printFlag);
    
    if(!arq || !pArqRef) {
        if(arq)
            fclose(arq);
        else
            fclose(pArqRef);
        printf("Erro de memória\n");
        return;
    }
    
    ItemABB temp = {0};
    int pos = 0;
    // Lê da referencia
    while(pos < tam) {
        fread(&temp.item, sizeof(TipoItem), 1, pArqRef);
        bench->transf++;
        temp.esq = temp.dir = -1;
        
        // Colocar no final do abb.bin
        fseek(arq, 0, SEEK_END);
        fwrite(&temp, sizeof(ItemABB), 1, arq);
        if(pos > 0)
            insereFilhos(arq, temp.item.chave, pos,bench);
        pos++;
    }

    fclose(arq);
    fclose(pArqRef);
}

int pesquisaABB(int chave, int situacao, Bench *bench, int printFlag, int tam) {
    criaArquivoABB(situacao, bench, printFlag, tam);

    FILE *arq = fopen("abb.bin", "rb");
    if (!arq) {
        printf("Erro ao abrir o arquivo!\n");
        return -1;
    }
    ItemABB pesq;
    int pos = 0;
    
    while (1) {
        fseek(arq, pos * sizeof(ItemABB), SEEK_SET);

        bench->transf++;
        if (fread(&pesq, sizeof(ItemABB), 1, arq) != 1) {           
            fclose(arq);
            break;
        }
        
        bench->comp++;
        if (chave < pesq.item.chave) {
            if (pesq.esq == -1) break;
            pos = pesq.esq;
        }
        else if (chave > pesq.item.chave) {
            bench->comp++;
            if (pesq.dir == -1) break;
            pos = pesq.dir;
        }
        else {
            bench->comp++;
            printItem(&pesq.item);
            fclose(arq);
            return 1;
        }
    }
    printf("Item nao encontrado.\n");
    fclose(arq);
    return -1;
}
