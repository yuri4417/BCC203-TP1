#ifndef STRUCT_H
#define STRUCT_H
#define ITENSPAGINA 4
#define MAXPAGINA 250000

// #define arq "arq.bin"
typedef struct {
    int metodo;
    int quantidade;
    int situacao;
    int chave;
    int imprimir;
} Config;


typedef struct{
    int chave;
    int posicao;
}TipoIndice;

typedef struct{
    int chave;
    long int dado1;
    char dado2[1001];
    char dado3[5001];
}TipoItem;

typedef struct {
    int esq;
    TipoItem item;
    int dir;
} ItemABB;

#endif // STRUCT_H
