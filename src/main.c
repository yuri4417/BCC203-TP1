#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "Executavel.h"

int main(int argc, char *argv[]){
    srand(time(NULL));
    Config cfg;
    if(!valida(argc, argv, &cfg))
        return -1;
    executar(&cfg);
    return 0;

}