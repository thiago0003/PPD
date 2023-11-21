/* 
 ** Universidade Federal de São Carlos
 ** Departamento de Computação
 ** Prof. Hélio Crestana Guardia
 */

/*
 ** Programa : soma dos elementos de um vetor
 ** Comando: 
 ** Objetivo: observar passagem de valores para função da thread.
              Nesse caso, usa-se vetor em que cada thread preenche 
              os resultados da sua operação. 
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h> 
#include <time.h>
#include <omp.h>

#define NELEM (1<<30)

int
main(int argc, char *argv[])
{
	int i;
	float *vet;
	double sum;
	long int nelem;
	unsigned int seedp;

	if(argc > 1)
		nelem = atoi(argv[1]);
	else
		nelem = NELEM;

	// alocação do vetor
	vet = (float *)malloc(nelem * sizeof(float));

	if (!vet) {
		perror("Erro na alocacao do vetor de elementos.");
		return EXIT_FAILURE;
	}

	#pragma omp parallel for
	for(i=0; i < nelem; i++) 
		vet[i] = (float)((float)rand_r(&seedp) / (float)RAND_MAX);

	sum = 0;

	#pragma omp parallel for reduction(+:sum)
	for(i=0; i < nelem; i++)
		sum += vet[i];

	printf("Soma: %f\n",sum);

	free(vet);

	return(0);
}


