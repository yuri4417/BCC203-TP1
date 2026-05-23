#ifndef EXECUTAVEL_H
#define EXECUTAVEL_H
#include "Struct.h"
typedef struct timespec Timer;
void timerStart(Timer *t);
double timerStop(Timer *pIni);
int valida(int argc, char *argv[], Config *cfg);
void executar(Config *cfg/*int flagTeste*/);
void printItem(TipoRegistro *item);
// void rodarTestes(Config *cfg);

#endif /* EXECUTAVEL_H */
