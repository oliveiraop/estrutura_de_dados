#ifndef __EstruturasDeDados__
#define __EstruturasDeDados__ 1

#define GRAU_ARVORE 2

typedef struct point 	{	int x;
							int y;
						} tPonto;
						
typedef struct	quadrante{ 	tPonto	pBase;
					int 	h,w;
					int 	nivel;
					int		cor;
					float	erro;
					
					struct quadrante *filho1;
					struct quadrante *filho2;
					struct quadrante *filho3;
					struct quadrante *filho4;
				} 	tQuadrante;
				
								
#endif		// __EstruturasDeDados__
