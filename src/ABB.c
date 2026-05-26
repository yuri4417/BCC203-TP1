#include <stdio.h>
#include <stdlib.h>
#include "ABB.h"
#include "Struct.h"
#include "Executavel.h"
#include "Arquivos.h"

//Insere um no na ABB externa
void insereFilhos(FILE *arq, int chave, int pos, Bench *bench) {
    ItemABB atual;

    //Comeca pela raiz
    int indiceAtual = 0;
    while (1) {
        //Vai para a posicao do no atual e le ele
        fseek(arq, indiceAtual * sizeof(ItemABB), SEEK_SET);
        if (fread(&atual, sizeof(ItemABB), 1, arq) != 1) {
            return;
        }   
        bench->transf++;
        
        //Verifica lado esquerdo
        bench->comp++;
        if (chave < atual.item.chave) {
            //Se nao tiver filhos e so inserir
            if (atual.esq == -1) {
                atual.esq = pos;
                //Atualiza o no no arquivo
                fseek(arq, indiceAtual * sizeof(ItemABB), SEEK_SET); //porque leu antes
                fwrite(&atual, sizeof(ItemABB), 1, arq);
                return; 
            } 
            //Se tiver filhos atualiza o endereco e continua a esquerda
            indiceAtual = atual.esq;
        } 
        //Verifica lado direito
        else {
            //Se nao tiver filhos e so inserir
            if (atual.dir == -1) {
                atual.dir = pos;

                //Atualiza o no no arquivo
                fseek(arq, indiceAtual * sizeof(ItemABB), SEEK_SET);
                fwrite(&atual, sizeof(ItemABB), 1, arq);
                return;
            }
            //Se tiver filhos atualiza o endereco e continua a direita
            indiceAtual = atual.dir;
        }
        
    }
}

// Cria arquivo da ABB externa
void criaArquivoABB(Bench *bench, int tam, FILE* pArqRef) {
    //Cria e abre o arquivo binario
    FILE *arq = fopen("./data/abb.bin", "w+b");
    
    if(!arq || !pArqRef) {
        if(arq)
            fclose(arq);
        else
            fclose(pArqRef);
        printf("Erro de memória\n");
        return;
    }
    
    //Buffer de leitura em blocos
    TipoItem *buffer = malloc(sizeof(TipoItem) * BLOCK_SIZE);
    if (!buffer) {
        printf("Erro de memória\n");
        fclose(arq);
        fclose(pArqRef);
        return;
    }
    int qtdRestante = tam;
    int toRead;

    //Posicao do novo no
    int pos = 0;

    //Le em blocos
    while (qtdRestante > 0) {

        //Define o tamanho da leitura
        toRead = (qtdRestante > BLOCK_SIZE) ? BLOCK_SIZE : qtdRestante;

        //Le bloco
        int lidos = fread(buffer, sizeof(TipoItem), toRead, pArqRef);
        bench->transf += lidos;

        //Insere cada item
        for (int i = 0; i < lidos; i++) {
            ItemABB temp = {0};
            temp.item = buffer[i];
            
            //Inicializa filhos
            temp.esq = temp.dir = -1;

            //Escreve os nos no arquivo
            fseek(arq, pos * sizeof(ItemABB), SEEK_SET);
            fwrite(&temp, sizeof(ItemABB), 1, arq);

            //Apenas se nao for a raiz (pos=0), ai insere os filhos
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

//Pesquisa um item na ABB Externa
int pesquisaABB(int chave,Bench *bench, int tam, FILE *pArqRef) {

    //Monta a ABB e abre ela para ler
    criaArquivoABB(bench, tam, pArqRef);
    FILE *arq = fopen("./data/abb.bin", "rb");
    if (!arq) {
        printf("Erro ao abrir o arquivo!\n");
        return -1;
    }
    ItemABB pesq;

    //Comeca pela raiz
    int pos = 0;
    while (1) {

        //Vai para o no atual
        fseek(arq, pos * sizeof(ItemABB), SEEK_SET);

        //Le o no
        if (fread(&pesq, sizeof(ItemABB), 1, arq) != 1) {           
            fclose(arq);
            return -1;
        }
        bench->transf++;
        
        //Vai para a esquerda se a chave for menor que o item procurado
        bench->comp++;
        if (chave < pesq.item.chave) {
            //Se nao tiver filho a esquerda dele, o item procurado nao exista
            if (pesq.esq == -1){
                printf("Item %d nao encontrado!\n",chave);
                fclose(arq);
                return -1;
            }
            //Se tiver um filho a esquerda eu atualizo a posicao e procuro nele agora
            pos = pesq.esq;
        }
        //Se nao for menor ou e maior ou acha o item
        else{
            //Vai para a direita se a chvae for maior que o item procurado
            bench->comp++;
            if (chave > pesq.item.chave) {
                //Se nao tiver filho a direita, o item procurado nao existe
                if (pesq.dir == -1){
                    printf("Item %d nao encontrado!\n",chave);
                    fclose(arq);
                    return -1;
                }
                //Se tiver filho a direita, atualiza a posicao e procura nele
                pos = pesq.dir;
            }
            //Se for encontrado o item printa ele
            else {
                printItem(&pesq.item);
                fclose(arq);
                return 1;
            }
        }
    }
    fclose(arq);
    return -1;
}
