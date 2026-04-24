#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Executavel.h"
#include "AcessoIdx.h"
#include "ABB.h"
#include "ArvoreB.h"
#include "ArvoreBEstrela.h"
#include "Arquivos.h"

int valida(int argc, char *argv[], Config *cfg){
    if (argc < 5)
        return 0;

    cfg->metodo = atoi(argv[1]);
    if(cfg->metodo < 1 || cfg->metodo > 4){ 
        printf("Metodo invalido. Use 1 para Acesso Indexado, 2 para ABB, 3 para Arvore B e 4 para Arvore B Estrela.\n");
        return 0;
    }

    cfg->quantidade = atoi(argv[2]);
    if(cfg->quantidade <= 0 || cfg->quantidade >250000){
        printf("Quantidade de itens deve ser maior que zero e menor que 1000000.\n");
        return 0;
    }

    cfg->situacao = atoi(argv[3]);
    if(cfg->situacao < 1 || cfg->situacao > 3){
        printf("Situacao invalida. Use 1 para arquivo ascendente, 2 para arquivo descendente e 3 para arquivo aleatorio.\n");
        return 0;
    }
    
    if (cfg->metodo == 1 && cfg->situacao != 1) {
        printf("Acesso Indexado necessita do arquivo ordenado para realizar a busca.\n");
        return 0;
    }
    
    cfg->chave = atoi(argv[4]);
    if(cfg->chave == 0){
        printf("Chave deve ser diferente de zero.\n");
        return 0;
    }

    cfg->imprimir = 0;
    if (argc == 6 && strcmp(argv[5], "[-P]") == 0) 
        cfg->imprimir = 1;

    else if(argc >= 6) {
        printf("Digite o comando correto: pesquisa <método> <quantidade> <situação> <chave> [-P]\n");
        return 0;
    }
    return 1;
}

void executar(Config *cfg){
    TipoIndice tabela[MAXPAGINA];
    TipoItem x; 

    criaArquivo(cfg->quantidade, cfg->situacao, cfg->imprimir);
    x.chave = cfg->chave;
    switch (cfg->metodo) {
        case 1:
            acessoIndexado(tabela, &x);
            break;
        case 2:
            pesquisaABB(x.chave, cfg->situacao);
            break;
        case 3:
            printf("Arvore B\n");
            if(cfg->imprimir){
                return; //Completar com a impressao
            }
            break;
        case 4:
            printf("Arvore B Estrela\n");
            if(cfg->imprimir){
                return; //Completar com a impressao
            }
            break;
    }
    
}