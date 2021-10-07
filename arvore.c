#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <GL/glut.h>

#include "EstruturasDeDados.h"
#include "winGL.h"
#include "arvore.h"
		
unsigned char* 	image = NULL;	// image file
int 			iHeight, 
				iWidth,
				iChannels;

bool 			desenha = false,
				quadGerada = false,
				modo = false;
float			erroTolerado = 0;
int				nivelTolerado,
				nivelMax;

tQuadrante		*raizImagem;

// ***********************************************
// ******                                   ******
// ***********************************************

void desenhaArvore(tQuadrante *raiz) {

	// rotina que deve ser implementada para visualizacao da arvore
	// utilize a rotina desenhaQuadrante(p0, p1, cor)
	// fornecendo os pontos inicial e final do quadrante e a sua cor
	// funcao do valor do pixel ou da regiao que voce quer desenhar 
	if (modo) {
		if (raiz->nivel == nivelTolerado || (raiz->nivel < nivelTolerado && raiz->filho1 == NULL)) {
			tPonto p1;
			
			p1.x = raiz->pBase.x + raiz->w;
			p1.y = raiz->pBase.y + raiz->h;	
			//printf(" Nivel: %d, Cor: %d\n", raiz->nivel, raiz->cor);
			//printf(" P0x: %d, P0y: %d, P1x: %d, P1y: %d\n", raiz->pBase.x, raiz->pBase.y, p1.x, p1.y);
			desenhaQuadrante(raiz->pBase, p1, raiz->cor);
			return;

		}
	} else {
		if (raiz->erro <= (erroTolerado/100.0)) {
			tPonto p1;
			
			p1.x = raiz->pBase.x + raiz->w;
			p1.y = raiz->pBase.y + raiz->h;	
			//printf(" Nivel: %d, Cor: %d\n", raiz->nivel, raiz->cor);
			//printf(" P0x: %d, P0y: %d, P1x: %d, P1y: %d\n", raiz->pBase.x, raiz->pBase.y, p1.x, p1.y);
			desenhaQuadrante(raiz->pBase, p1, raiz->cor);
			return;
		}
	}
	
	if (raiz->filho1 != NULL) desenhaArvore(raiz->filho1);
	if (raiz->filho2 != NULL) desenhaArvore(raiz->filho2);
	if (raiz->filho3 != NULL) desenhaArvore(raiz->filho3);
	if (raiz->filho4 != NULL) desenhaArvore(raiz->filho4);
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void GeraCor(tQuadrante *raiz) {
	int cont = 0, mediaCor;
	mediaCor = 0;
	if (raiz->h==1 && raiz->w==1) {
		raiz->cor = image[raiz->pBase.y * iWidth + raiz->pBase.x]; //valor do pixel
	} else { //calculo da media com relação aos filhos
		if (raiz->filho1 !=NULL) {
			cont++;
			mediaCor = mediaCor + raiz->filho1->cor;
		}
		if (raiz->filho2 !=NULL) {
			cont++;
			mediaCor = mediaCor + raiz->filho2->cor;
		}
		if (raiz->filho3 !=NULL) {
			cont++;
			mediaCor = mediaCor + raiz->filho3->cor;
		}
		if (raiz->filho4!=NULL) {
			cont++;
			mediaCor = mediaCor + raiz->filho4->cor;
		}
		mediaCor = mediaCor / cont;
		raiz->cor = mediaCor;
	}
	
}

tQuadrante* AlocarQuadrante() {
	tQuadrante *raiz;
	raiz = (tQuadrante *) malloc(sizeof(tQuadrante));
	raiz->filho1 = NULL;
	raiz->filho2 = NULL;
	raiz->filho3 = NULL;
	raiz->filho4 = NULL;
	return raiz;
	
}

float ErroRelativo(int media, int real) { // funcao simples para fazer o calculo do erro relativo
	float cima, mediaFloat, realFloat;
	mediaFloat = media;
	realFloat = real;
	if (mediaFloat > realFloat) {
		cima = mediaFloat - realFloat;
	} else if (mediaFloat < realFloat){
		cima = realFloat - mediaFloat;
	} else {
		return 0;
	}
	return (cima/realFloat);
}

void GeraErro(tQuadrante *raiz) { //funcao para atribuir o erro relativo do pixel.
	float max, teste, erroAcumulado;
	
	max = 0;
	erroAcumulado = 0;
	if (raiz->h == 1 && raiz->w ==1) {
		raiz->erro = 0;
	} else {
		if (raiz->filho1 != NULL) {
			teste = ErroRelativo(raiz->cor, raiz->filho1->cor);
			if (teste >= max) {
				max = teste;
			}
			if (raiz->filho1->erro > erroAcumulado) {
				erroAcumulado = raiz->filho1->erro;
			}
		}
		if (raiz->filho2 != NULL) {
			teste = ErroRelativo(raiz->cor, raiz->filho2->cor);
			if (teste >= max) {
				max = teste;
			}
			if (raiz->filho1->erro > erroAcumulado) {
				erroAcumulado = raiz->filho2->erro;
			}
		}
		if (raiz->filho3 != NULL) {
			teste = ErroRelativo( raiz->cor, raiz->filho3->cor);
			if (teste >= max) {
				max = teste;
			}
			if (raiz->filho1->erro > erroAcumulado) {
				erroAcumulado = raiz->filho3->erro;
			}
		}
		if (raiz->filho4 != NULL) {
			teste = ErroRelativo( raiz->cor, raiz->filho4->cor);
			if (teste >= max) {
				max = teste;
			}
			if (raiz->filho1->erro > erroAcumulado) {
				erroAcumulado = raiz->filho4->erro;
			}
		}
		raiz->erro = max + erroAcumulado;
	}
	
}



tQuadrante* montaArvore(int w, int h, int x, int y) {
	static int nivel = 0;
	tQuadrante *raiz;
	raiz = AlocarQuadrante();
	nivel++;
	if (w>1 && h==1) {
		raiz->filho1 = montaArvore(w/2 , h, x, y);
		if (w % 2 == 1) {
			raiz->filho2 = montaArvore(w/2 + 1, h, x + w/2, y);
		} else {
			raiz->filho2 = montaArvore(w/2, h, x + w/2, y);
		}
	} else if (h>1 && w==1) {
		if (h % 2 == 1) {
			raiz->filho1 = montaArvore(w, h/2 + 1, x, y + h/2);
		} else {
			raiz->filho1 = montaArvore(w, h/2, x, y + h/2);
		}
		raiz->filho2 = montaArvore(w, h/2, x, y);
	} else if (w>1 && h>1){
		if (w % 2 == 1 && h % 2 == 1) {
			raiz->filho1 = montaArvore(w/2, h/2 + 1, x, y + h/2);
			raiz->filho2 = montaArvore(w/2 + 1, h/2 + 1, x + w/2, y + h/2);
			raiz->filho3 = montaArvore(w/2 + 1, h/2, x + w/2, y);
			raiz->filho4 = montaArvore(w/2, h/2, x, y);
		} else if (w % 2 == 1) {
			raiz->filho1 = montaArvore(w/2, h/2, x, y + h/2);
			raiz->filho2 = montaArvore(w/2 + 1, h/2, x + w/2, y + h/2);
			raiz->filho3 = montaArvore(w/2 + 1, h/2, x + w/2, y);
			raiz->filho4 = montaArvore(w/2, h/2, x, y);
		} else if (h % 2 == 1) {
			raiz->filho1 = montaArvore(w/2, h/2 + 1, x, y + h/2);
			raiz->filho2 = montaArvore(w/2, h/2 + 1, x + w/2, y + h/2);
			raiz->filho3 = montaArvore(w/2, h/2, x + w/2, y);
			raiz->filho4 = montaArvore(w/2, h/2, x, y);
		} else {
			raiz->filho1 = montaArvore(w/2, h/2, x, y + h/2);
			raiz->filho2 = montaArvore(w/2, h/2, x + w/2, y + h/2);
			raiz->filho3 = montaArvore(w/2, h/2, x + w/2, y);
			raiz->filho4 = montaArvore(w/2, h/2, x, y);
		}
	}
	nivel--;
	if (nivel > nivelTolerado) {
		nivelMax = nivel;
		nivelTolerado = nivel;
	}
	raiz->h = h;
	raiz->w = w;
	raiz->pBase.x = x;
	raiz->pBase.y = y;
	raiz->nivel = nivel;
	GeraCor(raiz);
	GeraErro(raiz);
	// printf("Nivel: %d\n", nivel);
	// printf(" H: %d", h);
	// printf(" W: %d", w);
	// printf(" x: %d", x);
	// printf(" y: %d\n", y);
	// printf("Erro: %f", raiz->erro);
	// printf(" Cor: %d \n", raiz->cor);

	if (nivel == 0) printf("Quad tree gerada \n");
	return raiz;
	// codifique aqui a sua rotina de montagem da arvore 
}


	
/// ***********************************************************************
/// ** 
/// ***********************************************************************

void teclado(unsigned char key, int x, int y) {

	switch (key) {
		case 27		: 	exit(0);
						break;				
		case 'q'	:
		case 'Q'	: 	raizImagem = montaArvore(iWidth, iHeight, 0, 0);
						quadGerada = true;
						break;				
		case 'i'	:
		case 'I'	: 	if (quadGerada) desenha = !desenha;
						break;
		case 'o'	:
		case 'O'	:	modo = !modo;
						if (modo) printf("Compressao por nivel \n"); else printf("Compressao por erro \n");
						break;
		case '.'	:	if (nivelTolerado > 0) nivelTolerado--;
						printf("Nivel Tolerado: %d \n", nivelTolerado);
						break;
		case ','	:	if (nivelTolerado < nivelMax) nivelTolerado++;
						printf("Nivel Tolerado: %d \n", nivelTolerado);
						break;
		case 'k'	:
		case 'K'	:	erroTolerado = erroTolerado + 5;
						printf("Erro Tolerado: %.0f %%\n", erroTolerado);
						break;
		case 'l'	:
		case 'L'	: 	if (erroTolerado>0) erroTolerado = erroTolerado - 5;
						printf("Erro Tolerado: %.0f %%\n", erroTolerado);
						break;
		}
	glutPostRedisplay();
}
		
/// ***********************************************************************
/// **
/// ***********************************************************************

void mouse(int button, int button_state, int x, int y ) {

	if 	(button_state == GLUT_DOWN ) {
		switch (button) {
			
			case GLUT_LEFT_BUTTON	: 	printf("botao esquerdo x: %d y: %d\n", x , y);
										
										break;
	
			case GLUT_RIGHT_BUTTON	:	printf("botao direito?\n");
										break;
			}
		glutPostRedisplay();
		}
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

void desenho(void) {

    glClear (GL_COLOR_BUFFER_BIT); 
    
    glColor3f (1.0, 1.0, 1.0);
    
    if (desenha)
    	desenhaArvore(raizImagem);
    else
    	glDrawPixels( iWidth, iHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, image );
   	
    glutSwapBuffers ();
}

/// ***********************************************************************
/// ** 
/// ***********************************************************************

int main(int argc, char** argv) {
	
char* filename = "images/lena.png";
	char pausa;
    if (argc > 1)
		filename = argv[1];

	image = leImagem(filename);
			
	criaJanela(argc, argv);
	
    initOpenGL();
    
    initEventos();
    
	scanf("%c", pausa); 
    return 0;   
}
