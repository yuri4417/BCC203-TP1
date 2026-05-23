#ifndef STRUCT_H
#define STRUCT_H
#define BLOCK_SIZE 2000
#define ITENSPAGINA 50
#define MAXPAGINA 200000
#define M 2
#define QTD_TESTES 10




//Struct com as variaveis de controle na linha de execucao
typedef struct {
    int metodo;
    int quantidade;
    int situacao;
    int chave;
    int imprimir;
} Config;

//BENCH
typedef struct {
    long comp;
    long transf;
    double tempoExec;
}Bench;

//Struct para a tabela do Acesso Indexado
typedef struct{
    int chave;
    int posicao;
}TipoIndice;

//Item geral com a chave e os dados
typedef struct{
    int chave;
    long int dado1;
    char dado2[1001];
    char dado3[5001];
}TipoItem;

//Item da ABB com o endereco do filho a esq,dir e o item no meio
typedef struct {
    int esq;
    TipoItem item;
    int dir;
} ItemABB;

//ARVORE B
// Com um Tipo Pagina em que cada pagina contem o tamanho (n),
// Um vetor Registro com os itens que tem na pagina (r) e um vetor de apontador (p).  
typedef TipoItem TipoRegistro;
typedef struct TipoPagina* TipoApontador;
typedef struct TipoPagina {
    short n;
    TipoRegistro r[2 * M];
    TipoApontador p[2 * M + 1];
} TipoPagina;

//ARVORE B ESTRELA
//Com um Pt (Pagina Interna ou Externa)
//Uma uniao para caso for interna ser UU.U0 e ter a qtd de itens na Pagina (ni), um vetor de chaves (ri), e um vetor de apontadores (pi)
//Caso for externa ser UU.U1 e ter a qtd de iten na Pagina (ne) e um vetor de registros (re)
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
