#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "Executavel.h"
#include "Arquivos.h"
int main(int argc, char *argv[]){
    srand(time(NULL));
    
    Config cfg;
    Bench bench;
    if(!valida(argc, argv, &cfg))
        return -1;
    executar(&cfg, &bench);
    return 0;
}