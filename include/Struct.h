#ifndef STRUCT_H
#define STRUCT_H
#define ITENSPAGINA 4
#define MAXPAGINA 100

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

#endif // STRUCT_H
