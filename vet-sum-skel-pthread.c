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
#include <pthread.h> 
#include <limits.h> 
#include <time.h>
#include <stdatomic.h>

#define NELEM (1<<30)
#define N_THREADS 4


// Variável atômica. Pode ser incrementada sem mutex, usando opearação específica
// _Atomic double total = 0;
// Incremento: __atomic_fetch_add (& total, parcial, __ATOMIC_SEQ_CST);
float *vet;
long int samples_per_thread;
double total;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Funcao da thread: o que passar como parâmetro?
void *
soma(void *arg)
{
    long int j;
    double total_parcial;

    for(j = (long )arg * samples_per_thread; j < ((long )arg + 1) * samples_per_thread; j++)
        total_parcial += vet[j];

    pthread_mutex_lock(&mutex);
    total += total_parcial;
    pthread_mutex_unlock(&mutex);

    //total[(long) arg] = total_parcial;
    //printf("Parcial: %f, %lu, %ld\n", total[(long )arg], (long )arg, j);
	pthread_exit(NULL);
}

int
main(int argc, char *argv[])
{
	int i;
	double sum;
	long int nelem;
	unsigned int seedp;

	int num_threads = 4;
	// vetor de pthread_t para salvamento dos identificadores das threads
	pthread_t threads[N_THREADS]; 

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

	for(i=0; i < nelem; i++) 
		// vet[i] = (float)((float)rand() / (float)RAND_MAX);
		vet[i] = (float)((float)rand_r(&seedp) / (float)RAND_MAX);


	// soma sequencial dos elementos do vetor
	//sum = 0;
	//for(i=0; i < nelem; i++)
	//	sum += vet[i];
    
    //printf("Soma: %f\n",sum);
    //sum = 0;

    int status;
    samples_per_thread = (int) nelem/N_THREADS;
	// Loop de criacao das threads
	for (long i=0; i < num_threads; i++) {

		status = pthread_create(&threads[i], NULL, soma, (void *)i);

		if (status) {
			printf("Falha da criacao da thread %ld (%d)\n", i, status);
			return (EXIT_FAILURE);
		}
	}

    // long int ret;
    // double *ret;
	// loop de espera pelo término da execução das threads
	for (int i=0; i < num_threads; i++) {

		// join recebendo a soma parcial de cada thread
		status = pthread_join(threads[i], NULL);

        // sum += ret[i];
		if (status) {
			printf("Erro em pthread_join (%d)\n",status);
			break;
		}
	}

	printf("Soma: %f\n",total);

	// libera o vetor de ponteiros para as threads
	//free(threads);

	// libera o vetor de valores
	free(vet);

	return(0);
}


