#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "Executavel.h"
#include "Arquivos.h"
int main(int argc, char *argv[]){
    srand(time(NULL));
    Config cfg;
    if(!valida(argc, argv, &cfg))
        return -1;
    executar(&cfg);
    
    /*criaArquivos(1, 1);
    FILE *pArq = fopen("./data/arqCresc-1M.bin", "rb");
    TipoItem temp = {0};
    for (int i = 0; i < 10; i++) {
        fread(&temp, sizeof(TipoItem), 1, pArq);
        printf("%d\n", temp.chave);
    }
    */
    return 0;
}