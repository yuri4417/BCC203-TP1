#ifndef EXECUTAVEL_H
#define EXECUTAVEL_H
#include "Struct.h"
int valida(int argc, char *argv[], Config *cfg);
void executar(Config *cf, int flagTeste);
void printItem(TipoRegistro *item);
void rodarTestes(Config *cfg);
#endif /* EXECUTAVEL_H */
