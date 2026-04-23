#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Executavel.h"

int valida(int argc, char *argv[],int *imprimir){
    int metodo = atoi(argv[1]);
    if(metodo < 1 || metodo > 4){
        printf("Metodo invalido. Use 1 para Acesso Indexado, 2 para ABB, 3 para Arvore B e 4 para Arvore B Estrela.\n");
        return 0;
    }
    int quantidade = atoi(argv[2]);
    if(quantidade <= 0){
        printf("Quantidade de itens deve ser maior que zero.\n");
        return 0;
    }
    int situacao = atoi(argv[3]);
    if(situacao < 1 || situacao > 3){
        printf("Situacao invalida. Use 1 para arquivo ascendente, 2 para arquivo descendente e 3 para arquivo aleatorio.\n");
        return 0;
    }
    int chave = atoi(argv[4]);
    if(chave <= 0){
        printf("Chave deve ser maior que zero.\n");
        return 0;
    }
    *imprimir = 0;
    if (argc == 6 && strcmp(argv[5], "-P") == 0) {
        *imprimir = 1;
    }
    return 1;
}