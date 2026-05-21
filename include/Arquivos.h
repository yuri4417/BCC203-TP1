#ifndef ARQUIVOS_H
#define ARQUIVOS_H

FILE* criaArquivos(int situacao, int printFlag);
void geraAscendente(int qtdTotal, FILE* arq, int printFlag);
void geraDescendente (int qtdTotal, FILE* arq, int printFlag);
void geraRandom (int qtdTotal, FILE **arq, int printFlag);

#endif /* ARQUIVOS_H */
