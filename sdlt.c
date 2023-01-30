#ifdef __unix__
	#include <unistd.h>
#elif defined(_WIN32) || defined(WIN32)
	#include <windows.h>
#endif


#include <stdio.h>
#include <SDL2/SDL.h>
#include "xadrez.h"
#include <string.h>
#include <stdlib.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

struct peca {
        PEAO *p;
        TORRE *t;
        CAVALO *c;
        BISPO *b;
        RAINHA *r;
        REI *k;
        char path[20];
};


SDL_bool init(SDL_Window **window, SDL_Surface **gScreenSurface){
	SDL_bool flag = SDL_TRUE;

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		printf("SDL não inicializado, SDL_Error: %s\n", SDL_GetError() );
		flag = SDL_FALSE;
	}
	else{
		*window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(*window == NULL){
			printf("Janela nao criada, SDL_Error: %s\n", SDL_GetError());
			flag = SDL_FALSE;
		}
		else{
			*gScreenSurface = SDL_GetWindowSurface(*window);
		}
	
	}
	return flag;
}

SDL_bool loadMedia(SDL_Surface **image, char *path){
	SDL_bool flag = SDL_TRUE;

	*image = SDL_LoadBMP(path);
	if(*image == NULL){
		printf("Imagem \"%s\" nao carregada, SDL_Error : %s\n", path, SDL_GetError());
		flag = SDL_FALSE;
	}
	return flag;
}

void closeSurface(SDL_Surface **surface){
	SDL_FreeSurface (*surface);
	*surface = NULL;
}

void closeWindowQuit(SDL_Window **window){

	//Destroy window
	SDL_DestroyWindow (*window);
	*window = NULL;
	
		
	//Quit SDL subsystems
	SDL_Quit();
}

void clearScreen(SDL_Surface **gsurface, SDL_Window **gwindow){
	SDL_Surface *preto = NULL;
	if(loadMedia(&preto, "sprites/clear.bmp") == SDL_FALSE){
		printf("tela nao limpa\n");
		return;
	}
	SDL_BlitSurface( preto, NULL, *gsurface, NULL );
	return;
}

void initPecas(SDL_Surface **pecas){
	loadMedia(&(pecas[0]), "sprites/peaob.bmp");
	loadMedia(&(pecas[1]), "sprites/peaop.bmp");
	loadMedia(&(pecas[2]), "sprites/torreb.bmp");
	loadMedia(&(pecas[3]), "sprites/torrep.bmp");
	loadMedia(&(pecas[4]), "sprites/cavalob.bmp");
	loadMedia(&(pecas[5]), "sprites/cavalop.bmp");
	loadMedia(&(pecas[6]), "sprites/bispob.bmp");
	loadMedia(&(pecas[7]), "sprites/bispop.bmp");
	loadMedia(&(pecas[8]), "sprites/rainhab.bmp");
	loadMedia(&(pecas[9]), "sprites/rainhap.bmp");
	loadMedia(&(pecas[10]), "sprites/reib.bmp");	
	loadMedia(&(pecas[11]), "sprites/reip.bmp");
	loadMedia(&(pecas[12]), "sprites/branco.bmp");
	loadMedia(&(pecas[13]), "sprites/clear.bmp");
	loadMedia(&(pecas[14]), "sprites/pecasel.bmp");
	loadMedia(&(pecas[15]), "sprites/azul.bmp");
	loadMedia(&(pecas[16]), "sprites/ataque.bmp");
	loadMedia(&(pecas[17]), "sprites/cursor.bmp");

	return;


}

int pathPeca(PECA x){
	if(x.p != NULL){
		if(retornaCor(x) == 1){
			return 0;
		}
		else{
			return 1;
		}
	}
	if(x.t != NULL){
		if(retornaCor(x) == 1){
			return 2;
		}
		else{
			return 3;
		}
	}
	if(x.c != NULL){
                if(retornaCor(x) == 1){
                        return 4;
                }
                else{
                        return 5;
                }
        }
	if(x.b != NULL){
                if(retornaCor(x) == 1){
                        return 6;
                }
                else{
                        return 7;
                }
        }
	if(x.r != NULL){
                if(retornaCor(x) == 1){
                        return 8;
                }
                else{
                        return 9;
                }
        }
	if(x.k != NULL){
                if(retornaCor(x) == 1){
                        return 10;
                }
                else{
                        return 11;
                }
        }




}

void printarTabuleiro(PECA **tabuleiro, SDL_Window **gwindow, SDL_Surface **gsurface, SDL_Surface **pecas){
	SDL_Surface *image = NULL;
	SDL_Rect position;
	position.w = 60;
	position.h = 60;


	//IMPRIMINDO AS CASAS
	
        SDL_BlitSurface(pecas[13], NULL, *gsurface, NULL);
        

        for(int y = 0; y < 480; y +=60){
                if(y % 120 != 0){
                        for(int x = 60; x <= 480; x += 120){

                                position.x = x;
                                position.y = y;
                                SDL_BlitSurface(pecas[12], NULL, *gsurface, &position);

                        }
                }
                else{
                        for(int x = 0; x < 480; x +=120){
                                position.x = x;
                                position.y = y;
                                SDL_BlitSurface(pecas[12], NULL, *gsurface, &position);

                        }
                }

        }


	//IMPRIMINDO AS PECAS
	
	for(int y = 0; y < 8; y++){
		for(int x = 0; x < 8; x++){
			if(strcmp(tabuleiro[y][x].path, "default") == 0){
				continue;
			}
			else{
				position.x = x * 60;
				position.y = y * 60;
				SDL_BlitSurface(pecas[pathPeca(tabuleiro[y][x])], NULL, *gsurface, &position);
			}
		}
	}
	

	

}



void PrintMovimento(PECA **tabuleiro, SDL_Surface **gsurface,int x, int y, SDL_Surface **pecas){
	SDL_Rect pos;
	SDL_Surface *image = NULL;
	int mov, i, movx, movy, a, b;

	if(ExistePeca(tabuleiro[y][x]) == 0){
		return;
	}

	pos.x = x * 60;
	pos.y = y * 60;
	SDL_BlitSurface(pecas[14], NULL, *gsurface, &pos);

	SDL_BlitSurface(pecas[pathPeca(tabuleiro[y][x])], NULL, *gsurface, &pos);

        if(tabuleiro[y][x].p != NULL){
		//movimente peao 

                if(retornaCor(tabuleiro[y][x]) == 1){
		mov = -1;	
		}
		else{
			if(retornaCor(tabuleiro[y][x]) == 0)
				mov = 1;
			else{
				return;
			}
		}
			

                //verificando casas
                if(ExistePeca(tabuleiro[y+mov][x]) == 0){
			pos.x = (x) * 60;
			pos.y = (y + mov) * 60;
			SDL_BlitSurface(pecas[15], NULL, *gsurface, &pos);
                }
		if(primeiroMov(tabuleiro[y][x], 'p') == SDL_TRUE){
			if(!ExistePeca(tabuleiro[y + mov + mov][x])){
	                        pos.x = (x) * 60;
	                        pos.y = (y + mov + mov) * 60;
	                        SDL_BlitSurface(pecas[15], NULL, *gsurface, &pos);
			}
		}
		if(x - 1 >= 0 && ExistePeca(tabuleiro[y+mov][x-1]) == 1 && retornaCor(tabuleiro[y][x]) != retornaCor(tabuleiro[y+mov][x-1])){
			pos.x = (x-1) * 60;
			pos.y = (y + mov) * 60;
			SDL_BlitSurface(pecas[16], NULL, *gsurface, &pos);

			SDL_BlitSurface(pecas[pathPeca(tabuleiro[y+mov][x-1])], NULL, *gsurface, &pos);
		}
		if(x + 1 <= 7 && ExistePeca(tabuleiro[y+mov][x+1]) == 1 && retornaCor(tabuleiro[y][x]) != retornaCor(tabuleiro[y+mov][x+1])){
                        pos.x = (x+1) * 60;
                        pos.y = (y + mov) * 60;
                        SDL_BlitSurface(pecas[16], NULL, *gsurface, &pos);

                        SDL_BlitSurface(pecas[pathPeca(tabuleiro[y+mov][x+1])], NULL, *gsurface, &pos);
                }
		

                
	}
	

	else{
		if(tabuleiro[y][x].c != NULL){
		//movimente cavalo
		//Tem 8 movimentos possiveis
		//(x-2, y-1) - (x-1, y-2) - (x+1, y-2) - (x+2, y-1)
		i = 0;
		movy = 2;
		movx = 1;
		do{
				if(y + movy < 8 && y + movy >= 0){
				if(x + movx < 8 && x + movx >= 0 && ExistePeca(tabuleiro[y+movy][x+movx]) == 0){
		                        pos.x = (x + movx) * 60;
        	        	        pos.y = (y + movy) * 60;
	                        	SDL_BlitSurface(pecas[15], NULL, *gsurface, &pos);
				}
				else{
					if(x + movx < 8 && x + movx >= 0 && (retornaCor(tabuleiro[y+movy][x+movx]) != retornaCor(tabuleiro[y][x])) ){
						pos.x = (x + movx) * 60;
        	                                pos.y = (y + movy) * 60;
                	                        SDL_BlitSurface(pecas[16], NULL, *gsurface, &pos);

						SDL_BlitSurface(pecas[pathPeca(tabuleiro[y+movy][x+movx])], NULL, *gsurface, &pos);
					}
				}

				if(x - movx <= 7 && x - movx >= 0 && ExistePeca(tabuleiro[y+movy][x-movx]) == 0){
                                        pos.x = (x - movx) * 60;
                                        pos.y = (y + movy) * 60;
                                        SDL_BlitSurface(pecas[15], NULL, *gsurface, &pos);
                                }
                                else{
                                        if(x - movx <= 7 && x - movx >= 0 && retornaCor(tabuleiro[y+movy][x-movx]) != retornaCor(tabuleiro[y][x])){
                                                pos.x = (x - movx) * 60;
                                                pos.y = (y + movy) * 60;
                                                SDL_BlitSurface(pecas[16], NULL, *gsurface, &pos);

                                               
                                                SDL_BlitSurface(pecas[pathPeca(tabuleiro[y+movy][x-movx])], NULL, *gsurface, &pos);
						
                                        }
                                }
				}


	                
			
			i++;
			switch(i){
				case 1: movy = -2;
					movx = 1;
					break;
				case 2: movy = 1;
					movx = 2;
					break;
				case 3:	movy = -1;
					movx = 2;
					break;
			
			}
		}
		while(i < 4);


			
		
		}
		else{
			if(tabuleiro[y][x].b != NULL){
			//movimente bispo
			
				for(i = 1; i + y <= 7 && x + i <= 7; i++){
					if(ExistePeca(tabuleiro[y+i][x+i])==1){
						if(retornaCor(tabuleiro[y+i][x+i]) != retornaCor(tabuleiro[y][x])){
							
        	                                        pos.x = (x + i) * 60;
	                                                pos.y = (y + i) * 60;
                	                                SDL_BlitSurface(pecas[16], NULL, *gsurface, &pos);
                        	                        

							SDL_BlitSurface(pecas[pathPeca(tabuleiro[y+i][x+i])], NULL, *gsurface, &pos);
	                                                

						}
						break;
					}
					else{
	                                        pos.x = (x + i) * 60;
        	                                pos.y = (y + i) * 60;
                	                        SDL_BlitSurface(pecas[15], NULL, *gsurface, &pos);
                        	                
					}
                               
                               
				}
				for(i = 1; y - i >= 0 && x - i >= 0; i++){
					
					if(ExistePeca(tabuleiro[y-i][x-i])==1){
                                                if(retornaCor(tabuleiro[y-i][x-i]) != retornaCor(tabuleiro[y][x])){
                                                        
                                                        pos.x = (x - i) * 60;
                                                        pos.y = (y - i) * 60;
                                                        SDL_BlitSurface(pecas[16], NULL, *gsurface, &pos);
                                                        
                                                        SDL_BlitSurface(pecas[pathPeca(tabuleiro[y-i][x-i])], NULL, *gsurface, &pos);
                                                        

                                                }
                                                break;
                                        }
                                        else{
                                                
                                                pos.x = (x - i) * 60;
                                                pos.y = (y - i) * 60;
                                                SDL_BlitSurface(pecas[15], NULL, *gsurface, &pos);
                                                

					}
				}

				for(i = 1; y - i >= 0 && x + i <= 7; i++){
					if(ExistePeca(tabuleiro[y-i][x+i])==1){
                                                if(retornaCor(tabuleiro[y-i][x+i]) != retornaCor(tabuleiro[y][x])){
                                                       
                                                        pos.x = (x + i) * 60;
                                                        pos.y = (y - i) * 60;
                                                        SDL_BlitSurface(pecas[16], NULL, *gsurface, &pos);
                                                        

                                                       
                                                        SDL_BlitSurface(pecas[pathPeca(tabuleiro[y-i][x+i])], NULL, *gsurface, &pos);
                                                        

                                                }
                                                break;
                                        }
                                        else{
                                                
                                                pos.x = (x + i) * 60;
                                                pos.y = (y - i) * 60;
                                                SDL_BlitSurface(pecas[15], NULL, *gsurface, &pos);
                                                

                                        }
				}

				for(i = 1; i + y <= 7 && x - i >= 0; i++){
                                        if(ExistePeca(tabuleiro[y+i][x-i])==1){
                                                if(retornaCor(tabuleiro[y+i][x-i]) != retornaCor(tabuleiro[y][x])){
                                                        
                                                        pos.x = (x - i) * 60;
                                                        pos.y = (y + i) * 60;
                                                        SDL_BlitSurface(pecas[16], NULL, *gsurface, &pos);
                                                                                                              
                                                        SDL_BlitSurface(pecas[pathPeca(tabuleiro[y+i][x-i])], NULL, *gsurface, &pos);
                                                        

                                                }
                                                break;
                                        }
                                        else{
                                                
                                                pos.x = (x - i) * 60;
                                                pos.y = (y + i) * 60;
                                                SDL_BlitSurface(pecas[15], NULL, *gsurface, &pos);
                                                

                                        }
                                }

			}
			else{
				if(tabuleiro[y][x].t != NULL){
				//movimente torre
					for(i = 1; x + i <= 7;i++){
						if(ExistePeca(tabuleiro[y][x+i])==1){
        	                                        if(retornaCor(tabuleiro[y][x+i]) != retornaCor(tabuleiro[y][x])){
								pos.x = (x + i) * 60;
                        	                                pos.y = (y) * 60;
                                	                        SDL_BlitSurface(pecas[16], NULL, *gsurface, &pos);
                                        	                                                                	        
                                                        	SDL_BlitSurface(pecas[pathPeca(tabuleiro[y][x+i])], NULL, *gsurface, &pos);
	                                                        

                                                	}
                                                break;
                                        	}

	                                        else{
        	                                        
                	                                pos.x = (x + i) * 60;
                        	                        pos.y = (y) * 60;
                                	                SDL_BlitSurface(pecas[15], NULL, *gsurface, &pos);
                                        	        

						}
					}

					for(i = 1; x - i >= 0;i++){
                                                if(ExistePeca(tabuleiro[y][x-i])==1){
                                                        if(retornaCor(tabuleiro[y][x-i]) != retornaCor(tabuleiro[y][x])){
                                                                
                                                                pos.x = (x - i) * 60;
                                                                pos.y = (y) * 60;
                                                                SDL_BlitSurface(pecas[16], NULL, *gsurface, &pos);
                                                                                                                                
                                                                SDL_BlitSurface(pecas[pathPeca(tabuleiro[y][x-i])], NULL, *gsurface, &pos);
                                                               

                                                        }
                                                break;
                                                }

                                                else{
                                                        
                                                        pos.x = (x - i) * 60;
                                                        pos.y = (y) * 60;
                                                        SDL_BlitSurface(pecas[15], NULL, *gsurface, &pos);
                                                        

                                                }
                                        }

					for(i = 1; y - i >= 0;i++){
                                                if(ExistePeca(tabuleiro[y-i][x])==1){
                                                        if(retornaCor(tabuleiro[y-i][x]) != retornaCor(tabuleiro[y][x])){
                                                                
                                                                pos.x = (x) * 60;
                                                                pos.y = (y - i) * 60;
                                                                SDL_BlitSurface(pecas[16], NULL, *gsurface, &pos);
                                                               

                                                                
                                                                SDL_BlitSurface(pecas[pathPeca(tabuleiro[y-i][x])], NULL, *gsurface, &pos);
                                                                

                                                        }
                                                break;
                                                }

                                                else{
                                                        
                                                        pos.x = (x) * 60;
                                                        pos.y = (y - i) * 60;
                                                        SDL_BlitSurface(pecas[15], NULL, *gsurface, &pos);
                                                        

                                                }
                                        }

					for(i = 1; y + i <= 7;i++){
                                                if(ExistePeca(tabuleiro[y+i][x])==1){
                                                        if(retornaCor(tabuleiro[y+i][x]) != retornaCor(tabuleiro[y][x])){
                                                                
                                                                pos.x = (x) * 60;
                                                                pos.y = (y + i) * 60;
                                                                SDL_BlitSurface(pecas[16], NULL, *gsurface, &pos);
                                                                

                                                                
                                                                SDL_BlitSurface(pecas[pathPeca(tabuleiro[y+i][x])], NULL, *gsurface, &pos);
                                                                

                                                        }
                                                break;
                                                }

                                                else{
                                                       
                                                        pos.x = (x) * 60;
                                                        pos.y = (y + i) * 60;
                                                        SDL_BlitSurface(pecas[15], NULL, *gsurface, &pos);
                                                        

                                                }
                                        }


				
				}
				else{
					if(tabuleiro[y][x].r != NULL){
					//movimente rainha
						i=0;
						movx = 0;
						movy = 1;
						while(i<8){
							a=movx;
							b=movy;
							for(; x + a >= 0 && x + a <= 7 && y + b >= 0 && y + b <= 7; a+=movx , b+=movy){
		                                                if(ExistePeca(tabuleiro[y+b][x+a])==1){
        		                                                if(retornaCor(tabuleiro[y+b][x+a]) != retornaCor(tabuleiro[y][x])){
                		                                                
                        		                                        pos.x = (x + a) * 60;
                                		                                pos.y = (y + b) * 60;
                                        		                        SDL_BlitSurface(pecas[16], NULL, *gsurface, &pos);
                                                		                

                                                        		        
                                                                		SDL_BlitSurface(pecas[pathPeca(tabuleiro[y+b][x+a])], NULL, *gsurface, &pos);
	                                                                	closeSurface(&image);

                                                        		}
                                                		break;
	                                                	}

	                                                	else{	
	                                                        	
        	                                        	        pos.x = (x + a) * 60;
                	                        	                pos.y = (y + b) * 60;
                        	        	                        SDL_BlitSurface(pecas[15], NULL, *gsurface, &pos);
	                                	                        

                                                		}
                                        		}

							i++;
							switch(i){
								case 1 :	movx= 1;
										movy= 1;
										break;

								case 2 :	movx = 1;
										movy = 0;
										break;

								case 3 :        movx = 1;
                                                                                movy = -1;
                                                                                break;

								case 4 :        movx = 0;
                                                                                movy = -1;
                                                                                break;

								case 5 :        movx = -1;
                                                                                movy = -1;
                                                                                break;

								case 6 :        movx = -1;
                                                                                movy = 0;
                                                                                break;

								case 7 :        movx = -1;
                                                                                movy = 1;
                                                                                break;



							}
						}
					}
					else{
						if(tabuleiro[y][x].k != NULL){
						//movimente rei

							i=0;
        	                                        movx = 0;
                	                                movy = 1;
                        	                        while(i<8){
								a=movx;
								b=movy;
								
                                                	       	if(x + a >= 0 && x + a <= 7 && y + b >= 0 && y + b <= 7){
                                                        	        if(ExistePeca(tabuleiro[y+b][x+a])==1){
                                                                	        if(retornaCor(tabuleiro[y+b][x+a]) != retornaCor(tabuleiro[y][x])){
                                                                        	        
                                                                                	pos.x = (x + a) * 60;
	                                                                                pos.y = (y + b) * 60;
        	                                                                        SDL_BlitSurface(pecas[16], NULL, *gsurface, &pos);
                	                                                                
	
        	                                                                        
                	                                                                SDL_BlitSurface(pecas[pathPeca(tabuleiro[y+b][x+a])], NULL, *gsurface, &pos);
                        	                                                        
	
        	                                                                }
                	                                                }
	
        	                                                        else{
                	                                                        
                        	                                                pos.x = (x + a) * 60;
                                	                                        pos.y = (y + b) * 60;
                                        	                                SDL_BlitSurface(pecas[15], NULL, *gsurface, &pos);
                                                	                        
	
        	                                                        }
								}

                        	                                i++;
                                	                        switch(i){
                                        	                        case 1 :        movx= 1;
                                                	                                movy= 1;
                                                        	                        break;
	
        	                                                        case 2 :        movx = 1;
                	                                                                movy = 0;
                        	                                                        break;
	
        	                                                        case 3 :        movx = 1;
                	                                                                movy = -1;
                        	                                                        break;
	
        	                                                        case 4 :        movx = 0;
                	                                                                movy = -1;
                        	                                                        break;
	
        	                                                        case 5 :        movx = -1;
                	                                                                movy = -1;
                        	                                                        break;
	
        	                                                        case 6 :        movx = -1;
                	                                                                movy = 0;
        	                                                                        break;
	
                	                                                case 7 :        movx = -1;
                        	                                                        movy = 1;
											break;
                                                                                       
								
								}
						
						
							}

					}
					
				}
			}
			}
		}
	}
}

int cursor(int x, int y, SDL_bool *flag, SDL_Surface **gsurface, SDL_Window **gwindow, SDL_Surface **pecas){
	SDL_Event event;
	SDL_Rect pos;
	


	
	pos.x = x * 60;
	pos.y = y * 60;
	SDL_BlitSurface(pecas[17], NULL, *gsurface, &pos);
	SDL_UpdateWindowSurface( *gwindow );

	
	while( SDL_PollEvent( &event ) ){
        	switch( event.type ){
	            /* Look for a keypress */
        	    case SDL_KEYDOWN:
                	/* Check the SDLKey values and move change the coords */
	                switch( event.key.keysym.scancode ){
        	            case SDL_SCANCODE_LEFT:
				if(x!=0){
					--x;
				}
				

                        	break;

	                    case SDL_SCANCODE_RIGHT:
        	                if(x!=7){
					++x;		
				}
				

                	        break;

	                    case SDL_SCANCODE_UP:
				if(y!=0){
	        	                --y;
				}
				

                	        break;

	                    case SDL_SCANCODE_DOWN:
				if(y!=7){
        	                	++y;
				}
			

                	        break;
			   
			    case SDL_SCANCODE_RETURN:
				*flag = SDL_TRUE;

				break;

			    case SDL_SCANCODE_X:
				return -20;
				break;
				
			    case SDL_SCANCODE_D:
				return -15;
				break;
				
	                    default:
        	                break;
                	}
			break;
            }
    }
    return ((x * 10) + y);
}

SDL_bool MovimentoValido(PECA **tabuleiro, int x, int y, int Xnovo, int Ynovo){
	int mov, movx, movy, a, b;
	SDL_bool valido = SDL_FALSE;

	if(Xnovo == x && Ynovo == y){
		//nao ha movimento a ser feito
		return SDL_FALSE;
	}

	if(tabuleiro[y][x].p != NULL){
		if(retornaCor(tabuleiro[y][x]) == 0){
			mov = 1;
		}
		else{
			mov = -1;
		}

		if(x == Xnovo){
			if(y + mov == Ynovo){
				if(ExistePeca(tabuleiro[Ynovo][x]) == 0){
					valido = SDL_TRUE;

				}
			}
			else{
				if(y + mov + mov == Ynovo){
					if(primeiroMov(tabuleiro[y][x], 'p') == SDL_TRUE && ExistePeca(tabuleiro[y+mov][x])==0 && ExistePeca(tabuleiro[Ynovo][x])==0){
					valido = SDL_TRUE;
					}
				
				}
			}
		}
		else{
			if(x - 1 == Xnovo && y+mov == Ynovo){
				if(ExistePeca(tabuleiro[Ynovo][Xnovo])==1 && retornaCor(tabuleiro[Ynovo][Xnovo])!=retornaCor(tabuleiro[y][x]) ){
					valido = SDL_TRUE;
				}

			}
			else{
				if(x + 1 == Xnovo && y+mov == Ynovo){
					if(ExistePeca(tabuleiro[Ynovo][Xnovo])==1 && retornaCor(tabuleiro[Ynovo][Xnovo])!=retornaCor(tabuleiro[y][x])){
						valido = SDL_TRUE;
					}
				}
			}
		}


	}
	else{
		if(tabuleiro[y][x].c != NULL){
			if((Ynovo == y + 2 && (Xnovo == x+1 || Xnovo == x -1)) || (Ynovo == y - 2 && ((Xnovo == x+1) || (Xnovo == x -1)))  || (Ynovo == y + 1 && (Xnovo == x + 2 || Xnovo == x - 2)) || (Ynovo == y - 1 && (Xnovo == x +2 || Xnovo == x - 2)) && retornaCor(tabuleiro[Ynovo][Xnovo])!=retornaCor(tabuleiro[y][x])){
				valido = SDL_TRUE;
			}

			





		}
		else{
			if(tabuleiro[y][x].b != NULL){
				if(abs(Xnovo - x) == abs(Ynovo - y)){
					if(Xnovo - x < 0){
						movx = -1;
					}
					else{
						movx = 1;
					}

					if(Ynovo - y < 0){
						movy = -1;
					}
					else{
						movy = 1;
					}
					
					a = x + movx;
					b = y + movy;

					while(a != Xnovo){

						if(ExistePeca(tabuleiro[b][a])==1){
							//Ha uma peca no meio do caminho, movimento invalido
							return SDL_FALSE;
						}
						a += movx;
						b += movy;
					}
					valido = SDL_TRUE;
					if(retornaCor(tabuleiro[Ynovo][Xnovo]) == retornaCor(tabuleiro[y][x])){
                         		       return SDL_FALSE;
                        		}

				}
				else{
					return SDL_FALSE;
				}


			}
			else{
				if(tabuleiro[y][x].t != NULL){
					//movimente torre
					if(Xnovo == x){
						movx = 0;
						if(Ynovo - y > 0){
							movy = 1;
						}
						else{
							movy = -1;
						}
					}
					else{
						if(Ynovo == y){
							movy = 0;
							if(Xnovo - x > 0){
								movx = 1;
							}
							else{
								movx = -1;
							}
						}
					}
					if(Xnovo != x && Ynovo != y){
						return SDL_FALSE;
					}
					a = x + movx;
					b = y + movy;

					while(a != Xnovo || b != Ynovo){
						if(a != Xnovo && b != Ynovo && ExistePeca(tabuleiro[b][a])==1){
							//ha uma peca no caminho
							return SDL_FALSE;
						}
						a += movx;
						b += movy;
					
					}
					valido = SDL_TRUE;
					if(retornaCor(tabuleiro[Ynovo][Xnovo]) == retornaCor(tabuleiro[y][x])){
                         		       return SDL_FALSE;
                        		}


				}
				else{
					if(tabuleiro[y][x].r != NULL){
						//movimente rainha
						if(abs(Ynovo - y) == abs(Xnovo - x)){
							//movimento de bispo
							if(Xnovo - x < 0){
		                                                movx = -1;
                		                        }
                                        		else{
                                	                	movx = 1;
		                                        }
	
        		                                if(Ynovo - y < 0){
                        		                        movy = -1;
                                        		}
		                                        else{
                		                                movy = 1;
                                		        }

		                                        a = x + movx;
                		                        b = y + movy;

                                		        while(a != Xnovo){

                                                		if(a != Xnovo && ExistePeca(tabuleiro[b][a])==1){
		                                                        //Ha uma peca no meio do caminho, movimento invalido
                		                                        return SDL_FALSE;
		                                                }
                		                                a += movx;
                                		                b += movy;
		                                        }
                		                        valido = SDL_TRUE;
							if(retornaCor(tabuleiro[Ynovo][Xnovo]) == retornaCor(tabuleiro[y][x])){
                         				       return SDL_FALSE;
				                        }
                                		        


						}
						else{
							if(Xnovo == x || Ynovo == y){
								//movimento de torre
								if(Xnovo == x){
			                                                movx = 0;
                        		                        if(Ynovo - y > 0){
                                        		                movy = 1;
		                                                }
                		                                else{
                                		                        movy = -1;
                                                		}
			                              		}
			                                        else{
        	        		                                if(Ynovo == y){
                	                		                        movy = 0;
                        	                        		        if(Xnovo - x > 0){
		                	                                                movx = 1;
                		        	                                }
                                			                        else{
                                                			                movx = -1;
		                                                	        }
	                	                                	}
        		                                	}
	                        		      
                			                        a = x + movx;
                                			        b = y + movy;

		                        	                while(a != Xnovo || b != Ynovo){
                		                	                if((a != Xnovo || b != Ynovo) && ExistePeca(tabuleiro[b][a])==1){
                                		        	                //ha uma peca no caminho
                                                			        return SDL_FALSE;
			                                                }
        	        		                                a += movx;
                	                		                b += movy;
	
			                                        }
                			                        valido = SDL_TRUE;
								if(retornaCor(tabuleiro[Ynovo][Xnovo]) == retornaCor(tabuleiro[y][x])){
					                                return SDL_FALSE;
                        					}		

							}
						

						}


					}
					else{
						if(tabuleiro[y][x].k != NULL){
                                                        if(Ynovo - y > 1 || Ynovo - y < -1){
								return SDL_FALSE;
							}
							if(Xnovo - x > 1 || Xnovo - x < -1){
								return SDL_FALSE;
							}

                                                        valido = SDL_TRUE;
							if(retornaCor(tabuleiro[Ynovo][Xnovo]) == retornaCor(tabuleiro[y][x])){
                         				       return SDL_FALSE;
				                        }


                                                
                                                        
                                                }


						
						else{
							return valido;
						}
					}
				}
			}
		}
	}
	return valido;
}

void RealizaMovimento(SDL_bool valido, PECA *x, PECA *xnovo){
	if(valido == SDL_TRUE){
                        if(ExistePeca(*xnovo) == 1){
                                limparPeca(*xnovo);
				NullPecas(xnovo, 'd');
				
                        }
			if(x->p != NULL){

                        	xnovo->p = x->p;
	                        strcpy(xnovo->path, x->path);
        	                NullPecas(x, 'd');
                	        LimparPrimeiroMov(*xnovo, 'p');
				return;
			}

			if(x->t != NULL){
				xnovo->t = x->t;
                                strcpy(xnovo->path, x->path);
                                NullPecas(x, 'd');
                                LimparPrimeiroMov(*xnovo, 't');
                                return;

			}

			if(x->c != NULL){
				xnovo->c = x->c;
                                strcpy(xnovo->path, x->path);
                                NullPecas(x, 'd');
                                return;
			}

			if(x->b != NULL){
				xnovo->b = x->b;
                                strcpy(xnovo->path, x->path);
                                NullPecas(x, 'd');
                                return;
			}

			if(x->r != NULL){
				xnovo->r = x->r;
                                strcpy(xnovo->path, x->path);
                                NullPecas(x, 'd');
                                return;
			}

			if(x->k != NULL){
				xnovo->k = x->k;
                                strcpy(xnovo->path, x->path);
                                NullPecas(x, 'd');
                                LimparPrimeiroMov(*xnovo, 'k');
                                return;
			}

	}
}

			

int main(){
	SDL_Window *gwindow = NULL;
	SDL_Surface *gScreenSurface = NULL, *inicio = NULL;

	SDL_Surface *pecas[18];

	SDL_bool flag = SDL_TRUE, OKpassar;
	PECA **tabuleiro = NULL;
	int lado = 1, coord, pecaEscolhida, vitoria = 0;

	//Inicializacao do SDL
	if(init(&gwindow, &gScreenSurface) == SDL_FALSE){
		return -1;
	}

	//Carregando a tela de inicio
	if(loadMedia(&inicio, "sprites/Inicio.bmp") == SDL_FALSE){
		return -1;
	}


	//Carregando as imagens das pecas
	initPecas(pecas);

	SDL_BlitSurface( inicio, NULL, gScreenSurface, NULL );
	SDL_UpdateWindowSurface( gwindow );
	sleep(2);

	//Criacao do Tabuleiro
	tabuleiro = CriarTabuleiro(&flag);
	if(flag == SDL_FALSE){
		printf("criação falhou\n");
		return -1;
	}
	



        printarTabuleiro(tabuleiro, &gwindow, &gScreenSurface, pecas);
	

	while(vitoria == 0){
			if(lado == 0){
				//pretos
					do{
                                        flag = SDL_FALSE;
                                        OKpassar = SDL_FALSE;
                                        coord = 0;

                                        while(flag == SDL_FALSE){
                                                flag = SDL_FALSE;
                                                printarTabuleiro(tabuleiro, &gwindow, &gScreenSurface, pecas);
                                                coord = cursor(coord /10, coord % 10, &flag, &gScreenSurface, &gwindow, pecas);
                                                if(coord == -20){
                                                        coord = 0;
                                                        continue;
                                                }


                                                if(coord == -15){
                                                        break;
                                                }

                                                if(flag == SDL_TRUE){
                                                        if(ExistePeca(tabuleiro[coord%10][coord/10]) == 0 || retornaCor(tabuleiro[coord % 10][coord/10]) != 0){
                                                                printf("escolha uma peca preta\n");
                                                                flag = SDL_FALSE;
                                                                continue;
                                                        }
                                                }


                                                SDL_Delay(50);
                                        }
                                        if(coord == -15)
                                                break;

                                        pecaEscolhida = coord;

                                        while(OKpassar == SDL_FALSE){
                                                flag = SDL_FALSE;
                                                printarTabuleiro(tabuleiro, &gwindow, &gScreenSurface, pecas);
                                                PrintMovimento(tabuleiro, &gScreenSurface, pecaEscolhida/10, pecaEscolhida % 10, pecas);
                                                coord = cursor(coord/10, coord % 10, &flag, &gScreenSurface, &gwindow, pecas);
                                                if(coord == -15 || coord == -20)
                                                        break;
                                                if(flag == SDL_TRUE){
                                                        OKpassar = MovimentoValido(tabuleiro, pecaEscolhida /10, pecaEscolhida % 10, coord / 10, coord % 10);
							
                                                }
                                                SDL_Delay(50);
                                        }



                                        if(coord == -20){
                                                //desistencia de movimento
                                                coord = 0;
						flag = SDL_FALSE;
                                                continue;
                                        }

                                        if(OKpassar == SDL_TRUE)
                                                flag = OKpassar;

                                }while(coord != -15 && flag == SDL_FALSE);
                                if(coord == -15){
                                        //desistencia da partida
                                        vitoria = 1;
                                        break;
                                }

                                RealizaMovimento(OKpassar, &(tabuleiro[pecaEscolhida%10][pecaEscolhida/10]), &(tabuleiro[coord%10][coord/10]));
				vitoria = ExisteRei(tabuleiro);
                                lado = 1;





			}
			else{
				//brancos
				do{
					flag = SDL_FALSE;
					OKpassar = SDL_FALSE;
					coord = 7;
					
					while(flag == SDL_FALSE){
						flag = SDL_FALSE;
						printarTabuleiro(tabuleiro, &gwindow, &gScreenSurface, pecas);
						coord = cursor(coord /10, coord % 10, &flag, &gScreenSurface, &gwindow, pecas);
						if(coord == -20){
							coord = 7;
							continue;
						}
						

						if(coord == -15){
							break;
						}

						if(flag == SDL_TRUE){
							if(ExistePeca(tabuleiro[coord%10][coord/10]) == 0 || retornaCor(tabuleiro[coord % 10][coord/10]) != 1){
								printf("escolha uma peca branca\n");
								flag = SDL_FALSE;
								continue;
							}
						}

						
						SDL_Delay(20);
					}
					if(coord == -15)
						break;

					pecaEscolhida = coord;

					while(OKpassar == SDL_FALSE){
						flag = SDL_FALSE;
						printarTabuleiro(tabuleiro, &gwindow, &gScreenSurface, pecas);
						PrintMovimento(tabuleiro, &gScreenSurface, pecaEscolhida/10, pecaEscolhida % 10, pecas);
						coord = cursor(coord/10, coord % 10, &flag, &gScreenSurface, &gwindow, pecas);
						if(coord == -15 || coord == -20)
							break;
						if(flag == SDL_TRUE){
							OKpassar = MovimentoValido(tabuleiro, pecaEscolhida /10, pecaEscolhida % 10, coord / 10, coord % 10);
							
						}
						SDL_Delay(20);
					}
						
					

					if(coord == -20){
						//desistencia de movimento
						coord = 7;
						continue;
					}

					if(OKpassar == SDL_TRUE)
                                                flag = OKpassar;
					
				}while(coord != -15 && flag == SDL_FALSE);
				if(coord == -15){
					//desistencia da partida
					vitoria = -1;
					break;
				}

				RealizaMovimento(OKpassar, &(tabuleiro[pecaEscolhida%10][pecaEscolhida/10]), &(tabuleiro[coord%10][coord/10]));
				vitoria = ExisteRei(tabuleiro);
				lado = 0;

			}
	}



	if(vitoria == 1){
		printf("Vitoria dos Brancos\n");
	}
	else{
		printf("Vitoria dos Pretos\n");
	}
	
	

	tabuleiro = limparTabuleiro(tabuleiro);
        closeSurface(&gScreenSurface);
        closeWindowQuit(&gwindow);
	return 0;
}
