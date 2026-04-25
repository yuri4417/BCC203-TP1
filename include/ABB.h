#ifndef ABB_H
#define ABB_H

void criaArquivoABB(int situacao);
void insereFilhos(FILE *arq, int chave, int pos, int idx);
int pesquisaABB(int chave, int situacao);
#endif //ABB_H