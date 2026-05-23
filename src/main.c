#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "Executavel.h"
#include "Arquivos.h"
int main(int argc, char *argv[]){
    //Semente aleatoria como NULL
    srand(time(NULL));
    
    // Config cfg;
    // Bench bench;
    // //Valida a entrada e executa o programa
    // if(!valida(argc, argv, &cfg))
    //     return -1;
    // executar(&cfg, &bench);

    int qtdTestes = 5;
    Config cfg;
    cfg.imprimir = 0;
    cfg.metodo = ABB;
    cfg.quantidade = 100000;
    cfg.situacao = ARQCRESC;
    rodarTestes(qtdTestes, &cfg);
    cfg.quantidade = 100000;
    cfg.situacao = ARQDESC;
    rodarTestes(qtdTestes, &cfg);
    cfg.quantidade = 1000000;
    cfg.situacao = ARQCRESC;
    rodarTestes(qtdTestes, &cfg);
    cfg.quantidade = 1000000;
    cfg.situacao = ARQDESC;
    rodarTestes(qtdTestes, &cfg);
    return 0;
}