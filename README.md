# PPD
Programação Paralela Distribuída

## O que é?
Programação paralela trata da programação de computadores com múltiplos processadores e/ou de múltiplos computadores para resolver um problema mais rapidamente do que é possível com um único processador. 

Além de tratar da lógica do problema a resolver, na programação paralela o programador trata de como dividir as atividades para execução simultânea de suas partes, de forma coordenada.

A divisão das atividades pode prover resultados mais rápidos e pode também viabilizar a resolução de problemas que não são tratáveis em um único sistema, seja pela falta de recursos, como memória, ou pelo tempo que seria necessário para a execução.

## Paralelização 
Podemos dividir em estratégias como:
* Decomposição dos programas
* Distribuição dos programos e/ou dados
* Coordenação do processamento

O desempenho está condicionado à obtenção do melhor balanceamento entre processamento e comunicação, ou entre execução de instruções e acesso à memória para leitura e escrita. 

## Modelos de programação
* Memória Compartilhada: Baseada em tarefas, mais especificamente em threads (pthreads e openMP).
* Memória Distribuída: assumimos que as tarefas paralelas estão potencialmente localizadas em computadores distintos. Dessa forma a passagem de dados ou sincronização se dá via mensagens (MPI ou PVM). 
* Paralelismo de dados: usa-se elementos de processamento para realizar as mesmas operações simultaneamente em partes disjuntas dos dados (GPU-CUDA).

Quando é preciso criar um programa em paralelo, o progrmador estrutura o código de acordo com algum desses modelos ou com suas combinações.

### Sistemas multicore 
Para esse tipo de sistema o programador comummente usa um modelo baseado em threads para a memória compartilhada 

### Rack com vários computadores
É comum que o programador organize o código na forma de processos que se comunicam por passagem de mensagem.

## Granularidade
Podemos dividi-las em:
### Granularidade fina: 
Há potencialmente mais atividades que podem ser executadas simultaneamente. Se for preciso sincronizar as atividades, pode ser que as sobrecargas de comunicação entre esses blocos prejudiquem o desempenho da aplicação. 

* Tarefas paralelas relativamente pequenas em termos de código e tempo de execução.
* Comunicação são normalmente frequentes entre as tarefas, mas com pouco volume de dados.

### Granularidade grossa
* Tarefas relativamente grnades.
* Comunicação pouco frequente entre as tarefas mas, normalmente, com grande volume de dados.

## Características que favorecem a paralelização
* Modularidade: possibilidade de decomposição das atividades em componentes mais simples, desenvolvidos separadamente como módulos independentes e combinadas para a realização do programa. Projetos modulares reduzem a complexidade dos programas e facilitam a reutilização de código. 
* Concorrência: existência de atividades que podem ser executadas simultaneamente e que favorecem a paralelização.
* Escalabilidade: capacidade de variar a granulariade e o número de blocos paralelos, de forma a adaptar-se a ambientes com maiores números de processadores. 
* Localidade:"proximidade" dos dados manipulados nas operações de cada bloco paralelo, ou seja, se são os mesmos ou estão armazenados em locais próximos. 

### Localidade
A organização dos dados e seus particionamentos de forma a favorecer a localidade é comumente desejável. 

## Taxonomia de Flynn
* SISD: Single inctruction, single data stream
- Trata-se do caso de um computador convencional com uma única CPU, sem paralelismo de instruções ou dados. Dessa forma, há apenas um fluxo de execução de instruções que manipula um único dados de cada vez

* MISD: Multiple instruction, single data stream
- Computador hipotético, em que múltiplos processadores atuariam sobre um único fluxo de dados. Esse modelo pode ser empregado em caso de paralelismo para redundância.

* SIMD: Single instruction, multiple data streams
- Aplica-se para computadores que executam um único fluxo de instruções sobre múltiplos dados simultaneamente. Normalmente, há um grande número de processadores que executam a mesma operação de maneira coordenada (lock-step) sobre dados diferentes, como no caso de processadores vetoriais.

* MIMD: Multiple instruction, multiple data sreams
- Sistemas computacionais em que há multiplos processadores autônomos, que executam diferentes instruções sobre diferentes conjuntos de dados.

## Hyper-Threading
É uma tecnologia Intel para a execução simultânea de múltiplas threads num mesmo core. Com isso, o desempenho do processador é melhorado fazendo com que ele execute processamento útil quando estaria oscioso, como nas falhas de dados em chache (cache miss), branch misprediction ou na dependência de dados. Essa operação ocorre duplicando as partes específicas do processador relacionadas a manutenção do estado do hardware e dessa forma o SO "enxerga" cada um dos cores como um processador distinto, com seus próprios recursos de execução. 


## Threads

´pthread_create(pthread_t * restrict thread, const pthread_attr_t *restrict attr, void*(*start_routine)(void *), void *retrict arg)´

* O valor de retorno de criação: 0 = sucesso, outro valor = código do erro.

### Identificador de uma thread
* thread ID: esse valor é retornado como um dos parâmetros da chamada pthread_create. Além disso, pode obter o seu identificador usando a chamada pthread_self(). 

* OBS: Todas as threads de um processo compartilham o mesmo identificador de processo. 

### Encerramento da execução de threads
* Para a função main aguardar o retorno da thread, devemos utilizar a função pthread_join().

Qualquer thread de um processo pode utilizar pthread_join para esperar por qualquer outra thread deste processo. 

### Para a criação de um programa

* Incluir a biblioteca ´#include <pthread.h>´
* Definir o número de theads a ser criada ´#define NUM_THREADS 4´
* Dentro da função criar um vetor de threads ´pthread_t threads[NUM_THREADS];´
* Criar as threads ´int status = pthread_create(&threads[i], NULL, program, arg);´
* Podemos fazer uma verificação de falha na criação 
* Posteriormente esperamos o retorno da thread com ´int status = pthread_join(threads[i], NULL);´

* OBS: lembrando que o retorno 0 significa sucesso

### Passando parâmetro na ativação da thread
Como visto, a chamada pthread_create prevê apenas um parâmetro na função, sendo esse parâmetro do tipo (void *), em C isso pode ser utilizado para passar ponteiros para qualquer posição de meória que contenha qualquer tipo de dado.
Considerando a arquitetura 64 bits atuais, e um endereço de memória tem 64bits, logo também é possível passar como parâmetro para a função qualquer valor que caiba em 8 bytes. 

Para a passagem do parâmetro devemos fazer um cast no dados para o tipo (void *) ´status = pthread_create(&threads[t], NULL, hello_w, (void *)&args[t]);´

E na função receber esse parâmetro como ponteiro ´void * hello_w(void *arg)´. Dessa forma, basta convertermos para nosso tipo desejado ´int val = *(int *)arg;´ ou ´int * num = (int *)arg;´ e ´  int ind = *num;´. No caso da passagem do valor de forma direta, basta convertemos com ´(long int)num_int;´ pois não estamos passando uma posição de meória e sim um valor. 


Para pegarmos um valor de retorno da thread devemos passar como parâmetro no pthread_join o ponteiro de retorno ´status = pthread_join(th_struct, (void **)&retval));´, dessa forma na variável ´retval´ teremos o retorno da função. E dentro da função basta colocar o valor no ´	pthread_exit((long int *)34);´

### Compartilhamento de dados entre as Threads
De maineira geral, todas as variáveis que forem definidas como globais, ou seja, que foram declaradas fora dos escopo de qualquer função no código, serão compartilhadas por todas as threads deste processo. Porém se houver acessos simultâneos a esses dados poderá ser necessário uma sincronização.

### MUtex
As operações básicas em mutex são:
* int pthread_mutex_init(...);
* int pthread_mutex_destroy(...);
* int pthread_mutex_lock(...);
* int pthread_mutex_unlock(...);

### Semáforos
A ideia de ter semáforos com contadores de recursos associados é permitir que diversas operações sejam automaticamente liberadas, ao invés de implementar-se um mecanismo de alternância entre os atores que produzem e liberam recursos.

As funções para manipulação de semáforos são:
* int sem_init(...);
* int sem_destroy(...);
* int sem_post(...);
* int sem_wait(...);

## Incremento atômico 
Serve para identificar um bloco de código indivisível, ou seja, cujas instruções devem ser executadas em totalidade antes que o processador seja alocado a outra tarefa.
Algumas instruções que podem ser executadas de forma atômica em C/C++
* __atomic_fetch_add
* __atomic_fetch_sub
* __atomic_fetch_and
* __atomic_fetch_xor
* __atomic_fetch_or
* __atomic_fetch_nand

Também podemos declarar a variável do tipo atomica 
* ´_Atomic int atomic_global = 0;´

Dessa forma também é mapeado para o processador executala de forma atômica.
