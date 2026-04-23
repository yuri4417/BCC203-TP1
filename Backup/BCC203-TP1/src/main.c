#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "AcessoIdx.h"
#include "ABB.h"
#include "ArvoreB.h"
#include "ArvoreBEstrela.h"
#include "Arquivos.h"
#include "Executavel.h"

int main(int argc, char *argv[]){
    srand(time(NULL));
    TipoIndice tabela[MAXPAGINA];
    TipoItem x; 
    int imprimir;

    valida(argc, argv, &imprimir);
    criaArquivos(quantidade);
    x.chave = chave;
    switch (metodo) {
        case 1:
            acessoIndexado(tabela, &x);
            if(imprimir){
                return -1; //Completar com a impressao
            }
            break;
        case 2:
            printf("ABB\n");
            if(imprimir){
                return -1; //Completar com a impressao
            }
            break;
        case 3:
            printf("Arvore B\n");
            if(imprimir){
                return -1; //Completar com a impressao
            }
            break;
        case 4:
            printf("Arvore B Estrela\n");
            if(imprimir){
                return -1; //Completar com a impressao
            }
            break;
        default:
            return 0;
    }
    
    return 0;
}