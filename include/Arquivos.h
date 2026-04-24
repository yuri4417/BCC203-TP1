#ifndef ARQUIVOS_H
#define ARQUIVOS_H

void geraString(char *string, int tamString);
void criaArquivo(int qtdTotal, int situacao, int printFlag);
void geraAscendente(int qtdTotal, FILE* arq, int printFlag);
void geraDescendente (int qtdTotal, FILE* arq, int printFlag);
void geraRandom (int qtdTotal, FILE *arq, int printFlag);

#endif /* ARQUIVOS_H */
