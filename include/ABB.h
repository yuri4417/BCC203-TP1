#ifndef ABB_H
#define ABB_H
#include "Struct.h"

void criaArquivoABB(int situacao, Bench *bench, int printFlag, int tam);
void insereFilhos(FILE *arq, int chave, int pos, Bench *bench);
int pesquisaABB(int chave, int situacao, Bench *bench, int printFlag, int tam);
#endif //ABB_H