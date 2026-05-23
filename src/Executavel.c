#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Executavel.h"
#include "AcessoIdx.h"
#include "ABB.h"
#include "ArvoreB.h"
#include "ArvoreBEstrela.h"
#include "Struct.h"
#include "Arquivos.h"
#include <time.h>

//Macros para habilitar funcionalidades POSIX para as funcoes de tempo
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif


//Funcoes para calcular o tempo de execucao em segundos
void timerStart(Timer *t) {
    clock_gettime(CLOCK_MONOTONIC, t);
}
double timerStop(Timer *pIni) {
    struct timespec fim;
    clock_gettime(CLOCK_MONOTONIC, &fim);
    return (fim.tv_sec - pIni->tv_sec) + (fim.tv_nsec - pIni->tv_nsec) / 1e9;
}

//Funcao que valida entrada do programa
int valida(int argc, char *argv[], Config *cfg){
    //Verifica se o arquivo tem o tamanho certo
    if (argc < 5)
        return 0;

    //Valida qual metodo usar
    cfg->metodo = atoi(argv[1]);
    if(cfg->metodo < 1 || cfg->metodo > 4){ 
        printf("Metodo invalido. Use 1 para Acesso Indexado, 2 para ABB, 3 para Arvore B e 4 para Arvore B Estrela.\n");
        return 0;
    }

    //Valida a quantidade do arquivo
    cfg->quantidade = atoi(argv[2]);
    if(cfg->quantidade <= 0){
        printf("Digite uma quantidade válida.\n");
        return 0;
    }

    //Valida qual arquivo utilizar
    cfg->situacao = atoi(argv[3]);
    if(cfg->situacao < 1 || cfg->situacao > 3){
        printf("Situacao invalida. Use 1 para arquivo ascendente, 2 para arquivo descendente e 3 para arquivo aleatorio.\n");
        return 0;
    }
    
    //Acesso indexado nao pode ser arquivo desordenado
    if (cfg->metodo == 1 && cfg->situacao == 3) {
        printf("Acesso Indexado necessita do arquivo ordenado para realizar a busca.\n");
        return 0;
    }
    
    //Valida chave a ser procurada
    cfg->chave = atoi(argv[4]);
    if(cfg->chave == 0){
        printf("Chave deve ser diferente de zero.\n");
        return 0;
    }

    //Valida se tem [-P] para imprimir as chaves do arquivo
   
    if (argc == 6 && strcmp(argv[5], "[-P]") == 0) 
        cfg->imprimir = 1;

    //Verifica se tem o tamanho correto do comando mas algo escrito diferente
    else if(argc >= 6) {
        printf("Digite o comando correto: pesquisa <método> <quantidade> <situação> <chave> [-P]\n");
        return 0;
    }
    return 1;
}

void executar(Config *cfg, Bench *bench) {
    //Declara variaveis que sao usadas nos metodos
    Timer timer;   
    bench->comp = 0;
    bench->transf= 0;

    FILE *pArq =  criaArquivos(cfg->situacao, cfg->imprimir, cfg->quantidade);
    if (!pArq) {
        printf("Erro ao abrir o pArquivo.\n");
        return;
    } 
    timerStart(&timer); //Inicia o contador do tempo
    //Chama qual metodo usar
    switch (cfg->metodo) {
        case ACIDX:
            acessoIndexado(cfg->chave,cfg->situacao, bench, cfg->quantidade, pArq);
            break;
        case ABB:
            pesquisaABB(cfg->chave, bench, cfg->quantidade, pArq);
            break;
        case AB:
            arvoreB(cfg->chave, bench, cfg->quantidade, pArq);
            break;
        case ABEst:
            arvoreBEstrela(cfg->chave, bench, cfg->quantidade, pArq);
            break;
    }
    //Finaliza a contagem do tempo e printa as transf, comp e o tempo de execucao
    bench->tempoExec = timerStop(&timer);
    printf("Numero de transferencias: %ld\n", bench->transf);
    printf("Numero de comparacoes: %ld\n", bench->comp);
    printf("Tempo de execucao: %lf segundos\n", bench->tempoExec);
}

//Printa o item quando encontrado
void printItem(TipoRegistro *item) {
    printf("Item Encontrado!\n");
    printf("Chave: %d\n", item->chave);
    printf("Dado 1: %ld\n", item->dado1);
    printf("Dado 2: %s\n", item->dado2);
    printf("Dado 3: %s\n", item->dado3);
}

void rodarTestes(int numTestes, Config *cfg) {
    Bench *vecBench = calloc(numTestes, sizeof(Bench));
    if (!vecBench)
        return;

    for (int i = 0; i < numTestes; i++) {
        printf("\t ===== Teste %d =====\n", i+1);
        cfg->chave = (i+1) * (cfg->quantidade / numTestes);
        executar(cfg, &vecBench[i]);
        
    }
    for (int i = 1; i < numTestes; i++) {
        vecBench[0].comp += vecBench[i].comp;
        vecBench[0].transf += vecBench[i].transf;
        vecBench[0].tempoExec += vecBench[i].tempoExec;
    }
    double mediaComp =   (double) vecBench[0].comp / numTestes;
    double mediaTransf = (double) vecBench[0].transf / numTestes;
    double mediaTempo =  (double) vecBench[0].tempoExec / numTestes;
    printf("\t ===== %d Testes Realizados - Média =====\n", numTestes);
    printf("\t Comparacoes em media: %f\n", mediaComp);
    printf("\t Transferencias em media: %f\n", mediaTransf);
    printf("\t Tempo de execucao em media: %f s\n", mediaTempo);

    free(vecBench);
}