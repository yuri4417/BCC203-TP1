#ifndef ARVOREBESTRELA_H
#define ARVOREBESTRELA_H
void LiberaArvoreEstrela(TipoApontadorEstrela arvore);
void ImprimeEstrela(TipoApontadorEstrela arvore);
void PesquisaEstrela(TipoRegistro *x, TipoApontadorEstrela Ap, Bench *bench);
void InsereNaPaginaEstrela(TipoApontadorEstrela Ap, TipoRegistro Reg, TipoApontadorEstrela ApDir, Bench *bench);
void InsEstrela(TipoRegistro Reg, TipoApontadorEstrela *Ap, bool *Cresceu, TipoRegistro *RegRetorno, TipoApontadorEstrela *ApRetorno, Bench *bench);
void InsereEstrela(TipoRegistro Reg, TipoApontadorEstrela *Ap, Bench *bench) ;
void arvoreBEstrela(int chave, int situacao, Bench *bench, int printFlag, int tam) ;

#endif //ARVOREBESTRELA_H