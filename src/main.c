#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "Pesquisas.h"

void geraString(char *string, int tamString);
void criaArquivos(long tam);

int main(){
    srand(time(NULL));
    TipoIndice tabela[MAXTABELA];

    FILE *arq; 
    TipoItem x; 
    int pos;

    criaArquivos(20 + rand()%(MAXTABELA - 19));
    // Abre o arquivo de dados, lendo os itens armazenados no arquivo binario
    if ((arq = fopen("dados.bin","rb")) == NULL) {
        printf("Erro na abertura do arquivo\n"); 
        return 0;
    }

    // Leitura das primeiras posições de cada pagina
    pos = 0;
    
    while (fread(&x, sizeof(x), 1, arq) == 1) { 
        tabela[pos].chave = x.chave;
        tabela[pos].posicao = pos+1;
        pos++;
        fseek(arq, sizeof(TipoItem) * 3, SEEK_CUR);
    }
    
    fflush (stdout);  //Limpa o buffer para leitura da chave
    printf("Chave desejada:");
    scanf("%d",&x.chave);
    // Ativa a funcao de pesquisa, executando diferentes saidas caso livro encontrado ou nao
    if (acessoIndexado (tabela, pos, &x, arq))
        printf ("A chave %d foi localizado\n", x.chave);
    else
        printf ("A chave %d nao foi localizado\n",x.chave);
    fclose (arq);
    return 0;
}

void geraString(char *string, int tamString) { // preenche a string de maneira aleatória6
    for (int i = 0; i < tamString; i++) {
        string[i] = 'a'+rand()%('z' - 'a' + 1); // gera um caractere entre 'a' e 'z'
    }
    string[tamString] ='\0'; // termina a string
}

void criaArquivos(long tam) {
    tam *= 4;
    FILE *arq, *arqBin;
    arq = fopen("dados.txt", "w");
    arqBin = fopen("dados.bin", "wb");
    if(arq == NULL || arqBin == NULL){
        printf("Erro na criação dos arquivos\n");
        return;
    }

    TipoItem temp;
    int chaveAnterior = 0;
    for (int i = 0; i < tam; i++) {
        temp.chave = chaveAnterior + rand() % 10 + 1;
        temp.dado1 = rand();
        geraString(temp.dado2, 1000);
        geraString(temp.dado3, 5000);

        fprintf(arq, "%d %ld %s %s", temp.chave, temp.dado1, temp.dado2, temp.dado3);
        if(i != (tam-1))
            fprint(arq, "\n");
        fwrite(&temp, sizeof(TipoItem), 1, arqBin);
        
        chaveAnterior = temp.chave;
    }
    fclose(arq);
    fclose(arqBin);
}