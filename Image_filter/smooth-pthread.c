/* 
 * Universidade Federal de São Carlos
 * Departamento de Computação
 * Prof. Hélio Crestana Guardia
 */

/*
 * Programa : smooth (stencil de suavização pela média)
 * Objetivo: 
*/

/*
	*************** INCOMPLETO ********************
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h> 

#define WIDTH  512
#define HEIGHT 512
#define NUM_THREADS 4

int mr [WIDTH][HEIGHT];
int mg [WIDTH][HEIGHT];
int mb [WIDTH][HEIGHT];
int ma [WIDTH][HEIGHT];
int mr2 [WIDTH][HEIGHT];
int mg2 [WIDTH][HEIGHT];
int mb2 [WIDTH][HEIGHT];
int ma2 [WIDTH][HEIGHT];

void * pthread_filter(void *arg)
{
	int mr2_ [WIDTH][HEIGHT];
	int mg2_ [WIDTH][HEIGHT];
	int mb2_ [WIDTH][HEIGHT];
	int ma2_ [WIDTH][HEIGHT];

	for(int i = ((long)arg * HEIGHT) + 1; i < (((long)arg +1) * HEIGHT) -1; i++) {

		for(int j=1; j < WIDTH -1; j++) { 

			mr2_[i][j] = (mr[i-1][j-1] + mr[i-1][j] + mr[i-1][j+1] +
	      		         mr[i]  [j-1] + mr[i]  [j] + mr[i]  [j+1] +
      			         mr[i+1][j-1] + mr[i+1][j] + mr[i+1][j+1] ) / 9;

			mg2_[i][j] = (mg[i-1][j-1] + mg[i-1][j] + mg[i-1][j+1] +
	      		         mg[i]  [j-1] + mg[i]  [j] + mg[i]  [j+1] +
      			         mg[i+1][j-1] + mg[i+1][j] + mg[i+1][j+1] ) / 9;

			mb2_[i][j] = (mb[i-1][j-1] + mb[i-1][j] + mb[i-1][j+1] +
	      		         mb[i]  [j-1] + mb[i]  [j] + mb[i]  [j+1] +
      			         mb[i+1][j-1] + mb[i+1][j] + mb[i+1][j+1] ) / 9;

			ma2_[i][j] = (ma[i-1][j-1] + ma[i-1][j] + ma[i-1][j+1] +
	      		         ma[i]  [j-1] + ma[i]  [j] + ma[i]  [j+1] +
      			         ma[i+1][j-1] + ma[i+1][j] + ma[i+1][j+1] ) / 9;
			// replicar para mg2, mb2 e ma2...
		}
	}
}

int 
main(int argc, char **argv)
{
	int i,j;
	int fdi, fdo;

	int nlin = HEIGHT;
	int ncol = WIDTH;

	if(argc<2) {
		printf("Uso: %s nome_arquivo\n",argv[0]);
		exit(0);
	}
	if((fdi=open(argv[1],O_RDONLY))==-1) {
		printf("Erro na abertura do arquivo %s\n",argv[1]);
		exit(0);
	}

	// lê arquivo de imagem, supondo tamanho 512x512
	
	// (ao menos) 2 abordagens: 
	// - ler pixels byte a byte, colocando-os em matrizes separadas
	//	- ler pixels (32bits) e depois usar máscaras e rotações de bits para o processamento.

	// Ordem de leitura dos bytes (componentes do pixel): RGBA?

	for(i=0; i < nlin; i++) {
		for(j=0; j < ncol; j++) {
			read(fdi,&mr[i][j],1);
			read(fdi,&mg[i][j],1);
			read(fdi,&mb[i][j],1);
			read(fdi,&ma[i][j],1);
		}
	}
	close(fdi);

	// Aplicar filtro (estêncil), separadamente para componentes R, G, B e A

	// Tratar: linhas 0 e n-1; colunas 0 e n-1

	int status;
	pthread_t threads[NUM_THREADS]; 

	for (int i=0; i < NUM_THREADS; i++) {

		status = pthread_create(&threads[i], NULL, pthread_filter, (void *)i);

		if(status)
		{
			printf("Falha da criacao da thread %ld (%d)\n", i, status);
			return(EXIT_FAILURE);
		}
	}

	for (int i=0; i < NUM_THREADS; i++) {

		// join recebendo a soma parcial de cada thread
		status = pthread_join(threads[i], NULL);

        // sum += ret[i];
		if (status) {
			printf("Erro em pthread_join (%d)\n",status);
			break;
		}
	}

	// gravar imagem resultante
	fdo=open("out.rgba",O_WRONLY|O_CREAT,0644);

	(i=0; i < HEIGHT; i++) {
		for(j=0; j < WIDTH; j++) {
			write(fdo,&mr2[i][j],1);
			write(fdo,&mg2[i][j],1);
			write(fdo,&mb2[i][j],1);
			write(fdo,&ma2[i][j],1);
		}
	}

	close(fdo);
	
	return 0;
}


