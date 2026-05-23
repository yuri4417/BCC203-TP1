#include <stdio.h>
#include <stdlib.h>
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
            } // se tiver filhos atualiza o endereco
            indiceAtual = atual.esq;
        } 
        else {
            // se nao tiver filhos e so inserir
            if (atual.dir == -1) {
                atual.dir = pos;
                fseek(arq, indiceAtual * sizeof(ItemABB), SEEK_SET);
                fwrite(&atual, sizeof(ItemABB), 1, arq);
                return;
            }
            // se tiver filhos atualiza o endereco
            indiceAtual = atual.dir;
        }
        
    }
}

void criaArquivoABB(Bench *bench, int tam, FILE* pArqRef) {
    FILE *arq = fopen("./data/abb.bin", "w+b");
    
    if(!arq || !pArqRef) {
        if(arq)
            fclose(arq);
        else
            fclose(pArqRef);
        printf("Erro de memória\n");
        return;
    }
    
    TipoItem *buffer = malloc(sizeof(TipoItem) * BLOCK_SIZE);
    if (!buffer) {
        fclose(arq);
        fclose(pArqRef);
        return;
    }
    int qtdRestante = tam;
    int toRead;
    int pos = 0;
    while (qtdRestante > 0) {
        toRead = (qtdRestante > BLOCK_SIZE) ? BLOCK_SIZE : qtdRestante;
        int lidos = fread(buffer, sizeof(TipoItem), toRead, pArqRef);
        bench->transf += lidos;
        for (int i = 0; i < lidos; i++) {
            ItemABB temp = {0};
            temp.item = buffer[i];
            temp.esq = temp.dir = -1;

            fseek(arq, pos * sizeof(ItemABB), SEEK_SET);
            fwrite(&temp, sizeof(ItemABB), 1, arq);

            if (pos > 0)
                insereFilhos(arq, temp.item.chave, pos, bench);
            pos++;
        }
        qtdRestante -= lidos;
    }
    free(buffer);
    fclose(arq);
    fclose(pArqRef);
}

int pesquisaABB(int chave, int situacao, Bench *bench, int printFlag, int tam) {
    FILE *pArqRef = criaArquivos(situacao, printFlag, tam);
    criaArquivoABB(bench, tam, pArqRef);
    FILE *arq = fopen("./data/abb.bin", "rb");
    if (!arq) {
        printf("Erro ao abrir o arquivo!\n");
        return -1;
    }
    // Timer timer;
    // timerStart(&timer);
    ItemABB pesq;
    int pos = 0;
    
    while (1) {
        fseek(arq, pos * sizeof(ItemABB), SEEK_SET);

        bench->transf++;
        if (fread(&pesq, sizeof(ItemABB), 1, arq) != 1) {           
            fclose(arq);
            return -1;
        }
        
        bench->comp++;
        if (chave < pesq.item.chave) {
            if (pesq.esq == -1){
                fclose(arq);
                // bench->tempoExec = timerStop(&timer);
                return -1;
            }
            pos = pesq.esq;
        }
        else{
            bench->comp++;
            if (chave > pesq.item.chave) {
                if (pesq.dir == -1){
                    fclose(arq);
                    // bench->tempoExec = timerStop(&timer);
                    return -1;
                }
                pos = pesq.dir;
            }
            else {
                printItem(&pesq.item);
                fclose(arq);
                // bench->tempoExec = timerStop(&timer);
                return 1;
            }
        }
    }
    printf("Item nao encontrado.\n");
    fclose(arq);
    return -1;
}
