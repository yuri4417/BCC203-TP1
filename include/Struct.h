#ifndef STRUCT_H
#define STRUCT_H
#define ITENSPAGINA 50
#define MAXPAGINA 20000
#define M 2
#define QTD_TESTES 10
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

//BENCH
typedef struct {
    long comp;
    long transf;
    double tempoExec;
}Bench;

//ITEM GERAL
typedef struct{
    int chave;
    long int dado1;
    char dado2[1001];
    char dado3[5001];

}TipoItem;

//ARVORE ABB
typedef struct {
    int esq;
    TipoItem item;
    int dir;
} ItemABB;

//ARVORE B
typedef TipoItem TipoRegistro;

typedef TipoItem TipoRegistro;
typedef struct TipoPagina* TipoApontador;

typedef struct TipoPagina {
    short n;
    TipoRegistro r[2*M];
    TipoApontador p[2*M + 1];
} TipoPagina;



//ARVORE B ESTRELA
typedef enum {Interna, Externa} TipoIntExt;
typedef struct TipoPaginaEstrela* TipoApontadorEstrela;

typedef struct TipoPaginaEstrela {
    TipoIntExt Pt;
    union {
        struct {
            int ni;
            int ri[2*M];
            TipoApontadorEstrela pi[2*M + 1];
        } U0;
        struct {
            int ne;
            TipoRegistro re[4*M];
        } U1;
    } UU;
} TipoPaginaEstrela;

#endif // STRUCT_H
