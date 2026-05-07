#ifndef STRUCT_H
#define STRUCT_H
#define ITENSPAGINA 50
#define MAXPAGINA 20000
#define M 10

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
<<<<<<< HEAD
typedef TipoItem TipoRegistro;
=======
>>>>>>> ecf1304ba0b63ce1c8a41218570b0994d79aee8f

typedef TipoItem TipoRegistro;
typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina {
    short n;
<<<<<<< HEAD
    TipoRegistro r[MM];
    TipoApontador p[MM + 1];
} TipoPagina;

=======
    TipoRegistro r[2 * M];
    TipoApontador p[2 * M + 1];
} TipoPagina;


>>>>>>> ecf1304ba0b63ce1c8a41218570b0994d79aee8f

#endif // STRUCT_H
