#ifndef XADREZ_H
#define XADREZ_H

typedef struct peao PEAO ;
typedef struct torre TORRE ;
typedef struct cavalo CAVALO ;
typedef struct bispo BISPO ;
typedef struct rainha RAINHA ;
typedef struct rei REI ;
typedef struct peca PECA ;

PECA** CriarTabuleiro(SDL_bool *flag);
PECA** limparTabuleiro(PECA **tabuleiro);
int retornaCor(PECA x);
int ExistePeca(PECA t);
void NullPecas(PECA* elemento, char caso);
void LimparPrimeiroMov(PECA x, char tipo);
SDL_bool primeiroMov(PECA x, char tipo);
void limparPeca(PECA x);
int ExisteRei(PECA **tabuleiro);


#endif
