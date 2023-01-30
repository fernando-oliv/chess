#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>
#include "xadrez.h"
#include <string.h>

struct peao {
	int cor;
	//cor = 1 para peças brancas, 0 para peças pretas;
	SDL_bool primeiroMov;
};

struct torre {
	int cor;
	SDL_bool primeiroMov;
};

struct cavalo {
	int cor;
};

struct bispo {
	int cor;
};

struct rainha {
	int cor;
};

struct rei {
	int cor;
	SDL_bool primeiroMov;
};

struct peca {
	PEAO *p;
	TORRE *t;
	CAVALO *c;
	BISPO *b;
	RAINHA *r;
	REI *k;
	char path[20];
};

void NullPecas(PECA* elemento, char caso){
	switch(caso){
		case 'p':	elemento->t = NULL;
				elemento->c = NULL;
				elemento->b = NULL;
				elemento->r = NULL;
				elemento->k = NULL;
				
				break;

		case 't':	elemento->p = NULL;
                                elemento->c = NULL;
                                elemento->b = NULL;
                                elemento->r = NULL;
                                elemento->k = NULL;
				
				break;

		case 'c':	elemento->p = NULL;
                                elemento->t = NULL;
                                elemento->b = NULL;
                                elemento->r = NULL;
                                elemento->k = NULL;
				
				break;

		case 'b':	elemento->p = NULL;
                                elemento->t = NULL;
                                elemento->c = NULL;
                                elemento->r = NULL;
                                elemento->k = NULL;
				
				break;

		case 'r': 	elemento->p = NULL;
                                elemento->t = NULL;
                                elemento->c = NULL;
                                elemento->b = NULL;
                                elemento->k = NULL;
				
				break;

		case 'k':	elemento->p = NULL;
                                elemento->t = NULL;
                                elemento->c = NULL;
                                elemento->b = NULL;
                                elemento->r = NULL;
				break;

		default:	elemento->p = NULL;
                                elemento->t = NULL;
                                elemento->c = NULL;
                                elemento->b = NULL;
                                elemento->r = NULL;
				elemento->k = NULL;
				strcpy(elemento->path, "default");
                                break;

	}
	return;
}

PECA** CriarTabuleiro(SDL_bool *flag){
	PECA **tabuleiro = (PECA**)malloc(8 * sizeof(PECA*));
	int x, y;
	if(tabuleiro == NULL){
		printf("nao foi possivel alocar o tabuleiro\n");
		*flag = SDL_FALSE;
		return NULL;
	}

	for(int i = 0; i < 8; i++){
		tabuleiro[i] = (PECA*)malloc(8 * sizeof(PECA));
		if(tabuleiro[i] == NULL){
			printf("nao foi possivel alocar o tabuleiro\n");
			*flag = SDL_FALSE;
			return NULL;
		}
		for(int x =0; x < 8; x++){
			NullPecas(&(tabuleiro[i][x]), 'd');
		}
	}

	//ALOCANDO AS PECAS 
	//PEOES
	for(x = 0 ; x < 8; x++){
		tabuleiro[6][x].p = (PEAO*)malloc(1 * sizeof(PEAO));
		tabuleiro[1][x].p = (PEAO*)malloc(1 * sizeof(PEAO));
		if(tabuleiro[6][x].p == NULL || tabuleiro[1][x].p == NULL){
			printf("peoes nao alocados\n");
			*flag = SDL_FALSE;
			return limparTabuleiro(tabuleiro);
		}
		NullPecas(&(tabuleiro[6][x]), 'p');
		NullPecas(&(tabuleiro[1][x]), 'p');

		tabuleiro[6][x].p->cor = 1;
		tabuleiro[1][x].p->cor = 0;
		tabuleiro[6][x].p->primeiroMov = SDL_TRUE;
		tabuleiro[1][x].p->primeiroMov = SDL_TRUE;
		strcpy(tabuleiro[6][x].path, "sprites/peaob.bmp");
		strcpy(tabuleiro[1][x].path, "sprites/peaop.bmp");
		
	}

	//TORRES
	for(x = 0; x < 8; x+=7){
		tabuleiro[0][x].t = (TORRE*)malloc(1 * sizeof(TORRE));
		tabuleiro[7][x].t = (TORRE*)malloc(1 * sizeof(TORRE));
		if(tabuleiro[0][x].t == NULL || tabuleiro[7][x].t == NULL){
			printf("torres nao alocadas\n");
			*flag = SDL_FALSE;
			return limparTabuleiro(tabuleiro);
		}

		NullPecas(&(tabuleiro[0][x]), 't');
		NullPecas(&(tabuleiro[7][x]), 't');

		tabuleiro[0][x].t->cor = 0;
		tabuleiro[7][x].t->cor = 1;
		tabuleiro[0][x].t->primeiroMov = SDL_TRUE;
		tabuleiro[7][x].t->primeiroMov = SDL_TRUE;
		strcpy(tabuleiro[0][x].path, "sprites/torrep.bmp");
		strcpy(tabuleiro[7][x].path, "sprites/torreb.bmp");

	}

	//CAVALOS
	for(x = 1; x < 8; x += 5){
		tabuleiro[0][x].c = (CAVALO*)malloc(1 * sizeof(CAVALO));
		tabuleiro[7][x].c = (CAVALO*)malloc(1 * sizeof(CAVALO));
		if(tabuleiro[0][x].c == NULL || tabuleiro[7][x].c == NULL){
			printf("cavalos nao alocados\n");
			*flag = SDL_FALSE;
			return limparTabuleiro(tabuleiro);
		}

		NullPecas(&(tabuleiro[0][x]), 'c');
		NullPecas(&(tabuleiro[7][x]), 'c');

		tabuleiro[0][x].c->cor = 0;
		tabuleiro[7][x].c->cor = 1;
		strcpy(tabuleiro[0][x].path, "sprites/cavalop.bmp");
		strcpy(tabuleiro[7][x].path, "sprites/cavalob.bmp");


	}

	//BISPOS
	for(x = 2; x < 8; x += 3){
		tabuleiro[0][x].b = (BISPO*)malloc(1 * sizeof(BISPO)); 
		tabuleiro[7][x].b = (BISPO*)malloc(1 * sizeof(BISPO));
		if(tabuleiro[0][x].b == NULL || tabuleiro[7][x].b == NULL){
			printf("bispos nao alocados\n");
			*flag = SDL_FALSE;
			return limparTabuleiro(tabuleiro);
		}

		NullPecas(&(tabuleiro[0][x]), 'b');
		NullPecas(&(tabuleiro[7][x]), 'b');

		tabuleiro[0][x].b->cor = 0;
		tabuleiro[7][x].b->cor = 1;
		strcpy(tabuleiro[0][x].path, "sprites/bispop.bmp");
		strcpy(tabuleiro[7][x].path, "sprites/bispob.bmp");
	}

	//RAINHAS
	tabuleiro[0][3].r = (RAINHA*)malloc(1 * sizeof(RAINHA));
	tabuleiro[7][3].r = (RAINHA*)malloc(1 * sizeof(RAINHA));
	if(tabuleiro[0][3].r == NULL || tabuleiro[7][3].r == NULL){
		printf("rainhas nao alocadas\n");
		*flag = SDL_FALSE;
		return limparTabuleiro(tabuleiro);
	}
	
	NullPecas(&(tabuleiro[0][3]), 'r');
	NullPecas(&(tabuleiro[7][3]), 'r');

	tabuleiro[0][3].r->cor = 0;
	tabuleiro[7][3].r->cor = 1;
	strcpy(tabuleiro[0][3].path, "sprites/rainhap.bmp");
	strcpy(tabuleiro[7][3].path, "sprites/rainhab.bmp");

	//REIS
	tabuleiro[0][4].k = (REI*)malloc(1 * sizeof(REI));
	tabuleiro[7][4].k = (REI*)malloc(1 * sizeof(REI));
	if(tabuleiro[0][4].k == NULL || tabuleiro[7][4].k == NULL){
		printf("reis nao alocados\n");
		*flag = SDL_FALSE;
		return limparTabuleiro(tabuleiro);
	}

	NullPecas(&(tabuleiro[0][4]), 'k');
	NullPecas(&(tabuleiro[7][4]), 'k');

	tabuleiro[0][4].k->cor = 0;
	tabuleiro[7][4].k->cor = 1;
	strcpy(tabuleiro[0][4].path, "sprites/reip.bmp");
	strcpy(tabuleiro[7][4].path, "sprites/reib.bmp");


	return tabuleiro;
}

PECA** limparTabuleiro(PECA** tabuleiro){
	int x, y;
	for(y = 0; y < 8; y++){
		for(x = 0; x < 8; x ++){
			free(tabuleiro[y][x].p);
			free(tabuleiro[y][x].t);
			free(tabuleiro[y][x].c);
			free(tabuleiro[y][x].b);
			free(tabuleiro[y][x].r);
			free(tabuleiro[y][x].k);
		}
		free(tabuleiro[y]);
	}
	free(tabuleiro);
	return NULL;
}

int ExistePeca(PECA t){
        if(t.p == NULL && t.t == NULL && t.c == NULL && t.b == NULL && t.r == NULL && t.k == NULL){
                return 0;
        }
        return 1;
}


int retornaCor(PECA x){
	if(x.p != NULL){
		return x.p->cor;
	}
	if(x.t != NULL){
                return x.t->cor;
        }
	if(x.c != NULL){
                return x.c->cor;
        }
	if(x.b != NULL){
                return x.b->cor;
        }
	if(x.r != NULL){
                return x.r->cor;
        }
	if(x.k != NULL){
                return x.k->cor;
        }
	
	return -1;

}

SDL_bool primeiroMov(PECA x, char tipo){
	switch(tipo){
		case 'p': return x.p->primeiroMov;
			  break;
		case 't': return x.t->primeiroMov;
			  break;
		case 'k': return x.k->primeiroMov;
			  break;
	}
}

void limparPeca(PECA x){
	if(x.p != NULL){
		free(x.p);
	}
	else{
		if(x.t != NULL){
			free(x.t);
		}
		else{
			if(x.c != NULL){
				free(x.c);
			}
			else{
				if(x.b != NULL){
					free(x.b);
				}
				else{
					if(x.r != NULL){
						free(x.r);
					}
					else{
						if(x.k != NULL){
							free(x.k);
							return;
						}
					}
				}
			}
		}
	}
	
	return;
}

void LimparPrimeiroMov(PECA x, char tipo){
	switch(tipo){
		case 'p': x.p->primeiroMov = SDL_FALSE;
			  break;
		case 't': x.t->primeiroMov = SDL_FALSE;
			  break;
		case 'k': x.k->primeiroMov = SDL_FALSE;
			  break;
	
	}
	return;
}

int ExisteRei(PECA **tabuleiro){
	int x, y;
	SDL_bool reip=SDL_FALSE, reib=SDL_FALSE;
	for(y = 0; y < 8; y++){
		for(x = 0; x < 8; x++){
			if(tabuleiro[y][x].k != NULL){
				if(retornaCor(tabuleiro[y][x]) == 1){
					reib = SDL_TRUE;
				}
				else{
					reip = SDL_TRUE;
				}
			}
		}
	}

	if(reip == SDL_TRUE && reib == SDL_TRUE){
		return 0;
	}

	if(reib == SDL_TRUE){
		return 1;
	}

	if(reip == SDL_TRUE){
		return -1;
	}
}
