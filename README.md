# PPD Parte 1
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

`pthread_create(pthread_t * restrict thread, const pthread_attr_t *restrict attr, void*(*start_routine)(void *), void *retrict arg)`

* O valor de retorno de criação: 0 = sucesso, outro valor = código do erro.

### Identificador de uma thread
* thread ID: esse valor é retornado como um dos parâmetros da chamada pthread_create. Além disso, pode obter o seu identificador usando a chamada pthread_self(). 

* OBS: Todas as threads de um processo compartilham o mesmo identificador de processo. 

### Encerramento da execução de threads
* Para a função main aguardar o retorno da thread, devemos utilizar a função pthread_join().

Qualquer thread de um processo pode utilizar pthread_join para esperar por qualquer outra thread deste processo. 

### Para a criação de um programa

* Incluir a biblioteca `#include <pthread.h>`
* Definir o número de theads a ser criada `#define NUM_THREADS 4`
* Dentro da função criar um vetor de threads `pthread_t threads[NUM_THREADS];`
* Criar as threads `int status = pthread_create(&threads[i], NULL, program, arg);`
* Podemos fazer uma verificação de falha na criação 
* Posteriormente esperamos o retorno da thread com `int status = pthread_join(threads[i], NULL);`

* OBS: lembrando que o retorno 0 significa sucesso

### Passando parâmetro na ativação da thread
Como visto, a chamada pthread_create prevê apenas um parâmetro na função, sendo esse parâmetro do tipo (void *), em C isso pode ser utilizado para passar ponteiros para qualquer posição de meória que contenha qualquer tipo de dado.
Considerando a arquitetura 64 bits atuais, e um endereço de memória tem 64bits, logo também é possível passar como parâmetro para a função qualquer valor que caiba em 8 bytes. 

Para a passagem do parâmetro devemos fazer um cast no dados para o tipo (void *) `status = pthread_create(&threads[t], NULL, hello_w, (void *)&args[t]);`

E na função receber esse parâmetro como ponteiro `void * hello_w(void *arg)`. Dessa forma, basta convertermos para nosso tipo desejado `int val = *(int *)arg;` ou `int * num = (int *)arg;` e `  int ind = *num;`. No caso da passagem do valor de forma direta, basta convertemos com `(long int)num_int;` pois não estamos passando uma posição de meória e sim um valor. 


Para pegarmos um valor de retorno da thread devemos passar como parâmetro no pthread_join o ponteiro de retorno `status = pthread_join(th_struct, (void **)&retval));`, dessa forma na variável `retval` teremos o retorno da função. E dentro da função basta colocar o valor no `	pthread_exit((long int *)34);`

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
* `_Atomic int atomic_global = 0;`

Dessa forma também é mapeado para o processador executala de forma atômica.


## OpenMP (Open Multi-Processing)
É uma extensão de linguagem que permite ao programador inserir marcas no código para que o compilador o transforme em código paralelo.
Além disso, há suporte para paralelização de código usando processadores vetoriais, como GP-GPUs.

OpenMP permite:
* Criar times de threads para execução paralela de blocos de código
* Especificar como dividir (share) as atividades de um bloco de código entre os membros de um grupo
* Declarar variáveis compartilhadas e privadas
* Sincronizar threads e permitir que executem operações de maneira exclusiva
* Executar loops usando operações SIMD
* Utilizar dispositivos como GPUs para processamento vetorial

Esforço de paralelização de um programa com OpenMP resume-se, em geral, à identificação do paralelismo e não à reprogramação do código para implementar o paralelismo desejado.

* `omp_get_thread_num()`: retorna o número lógico de cada thread dentro do time que está executando a região paralela.
* `omp_get_num_threads()`: indica quantas threads há no time atual.
* `private(varaivel)` : indica uma lista de variáveis que serão privadas.
* `shared(variavel)` : indica que as variáveis listadas serão compartilhadas pelas threads do time.
* `reduction(variavel)` :
* Diretiva `Single`: Executa um trexo de código apenas por uma  thread. Theads no time que não executam a diretiva single esperam no final do código associado, exceto se a cláusula `nowait` for especificada.
* `sections`: permite especificar seções dentro da região paralela  que devem ser divididas entre as threads do time onde cada section é executada apenas uma vez por qualquer uma das threads do time 
* `#pragma omp atomic write`: esse tipo de acesso é traduzido pelo compilador em instruções do tipo ftech-and-add, impedindo múltiplas threads de escrever nessa localização ou lê-la simultaneamente.
* `#pragma omp parallel firstprivate(variavel)`: Neste caso, cada thread to time terá uma cópia da variável, privada, sendo que todas essas cópias terão como valor inicial o valor atual da variável na master thread.
* `reduction(:+variavel)`: Acumulador

## Processamento Vetorial e SIMD
### SIMD (Single Instruction, Multiple Data)
Modo de execução que está associado à realização simultânea da mesma operação sobre diferentes dados (paralelismo de dados). 

* Os principais obstáculos para a vetorização são o armazenamento dos dados de forma não contígua e as dependências de dados.
* Laços sem definição do número de iterações, em geral, também não são paralelizáveis.

A vetorização é mais facilmente aplicável a dados armazenados de maneira contígua na memória. Caso contrário, até pode ser possível copiar os dados para uma posição de memória que atenda essa consideração, mas essa operação nem sempre vale a pena.


### Como utilizar AVX
* `#include <xmmintrin.h>` : permite usar diretamente as instruções do processador como se fossem funções C.
* `__m128 va, vb, vresult;` : declaração dos ponteiros para os registradores SIMD
* `va = _mm_loadu_ps(a);    vb = _mm_loadu_ps(b); ` : carrega para os registradores SIMD o valor de "a" e "b".
* `vresult = _mm_add_ps(va, vb);`: executa a soma em paralelo.
* `_mm_storeu_ps(result, vresult);`: retorna o dado para uma variável do tipo array.


# PPD Parte 2

## MPI
MPI é uma biblioteca para programação paralela distribuída, comumente usada para criação de aplicações que se espalham por diferentes computadores interligados por rede.
O modelo de aplicação é comumente `SPMD`.

### Sockets 
API utilizada para comunicações entre processos em computadores distintos, podendo utilizad protocolos IPV4/IPV6 entre outros.
Cada socket na Internet usa apenas um dos chamados protoclos de transporte: TCP e UDP.
* TCP: É necessário que o SO estabeleça uma conexão com o socket do programa destino.
* UDP: Não há estabelecimento de conexão.

### Criação de processos
Multiplos processos que podem ser rodados em máquinas com SOs distintos, dessa forma a criação de processos pode ocorrer de maneira estática, em que os processos são especificados antes da execução, ou dinâmica, nos casos em que processos podem ser criados dinamicamente e suas execuções são iniciadas durante o processo de execução de outros processos.

Códigos a executar devem ser compilados antes da execução.

É comum ter um processo `mestre` que atua como controlador que ativa e coordena a execução dos processos `trabalhadores`

* Identificação única: permite identificar processos trabalhadores
* IDs: podem ser utilizados para modificar ações dos processos ou para determinar diferentes destinos para as mensagens 

### Single-Program, Multiple-Data (SPMD)
* É comum com processos criados estaticamente
* Diferentes programas (conjunto de atividades) são agrupadas em um único arquivo 
* Controles internos permitem selecionar diferentes partes do código para cada processo

### Multiple-Program, Multiple-Data (MPMD)
* Processos distintos são utilizados nos diferentes processadores
* Normalmente, 2 programas distintos são utilizados
* Um programa é executado no mestre e o outro nos demais trabalhadores
* IDs permitem personalizar a execução dos processos trabalhadores
* Criação dos processos trabalhadores pode ocorrer dinamicamente, durante a execução do processo mestre 

#### Primitivas básicas para comunicação
* `send(&msg, id_destino);`
* `recv(&msg, id_origem);`
O uso de endereço da mensagem para transmissão permite envio de mensgem de tamanhos variados e com dados de tipo simples ou complexo. 

### Estrutura dos programas 
Para utilizar MPI, é preciso fazer uma chamada à função `MPI_Init`, cabe a função executar as inicializações locais ao programa, como eventuais alocações de buffers internos, além de realizar comunicações com os demais nós lançados na ativação da aplicação. Tudo isso, contudo, é feito de forma transparente para a aplicação que invoca a chamada. Ao final do programa, é preciso executar a função `MPI_Finalize` que encerra as operações da biblioteca. 

### Grupo de processos 
Uma aplicação MPI é composta por um grupo de processos especificados na ativação do programa. 
Vários subgrupos podem ser formados, de acordo com a lógica da aplicação, mas há ao menos um subgrupo padrão `MPI_COMM_WORLD`. Esse grupo inclui todos os processos da apliação e, dentro desse grupo, cada processo é identificado por um `número lógico` chamdo `rank`, que varia de 0 a N-1 sendo o índice 0 atribuído para o processo que inicializou a aplicação. 

Com a biblioteca MPI não precisamos nos preocupar com o socket, uma vez que bastamos identificar o processo de acordo com seu número lógico que a biblioteca irá lidar com a comunicação. 

Uma vez compilado, um programa MPI pode ser ativado com números variados de processos; ou seja, pode-se experimentar usar mais ou menos processos em cada execução, sem que seja preciso recompilar o programa.

* `MPI_Comm_size(MPI_COMM_WORD, &numtasks)`: retorna o número de processos sendo usados nesta execução
* `MPI_Comm_rank(MPI_COMM_WORD, &ranck)` : retorna o rank deste processo em relação aos outros processos da aplicação
* `MPI_Get_processor_name(processor_name, &namelen)` : retorna o nome do processador 

### Compilação de programas
* inclusão do cabeçalho `<mpi.h>`
* para compilação utilizamos `mpicc program.c -o name`

### Ativação de programas MPI 
É comumente feita com os comandos `mpirun` ou `orterun` ou `mpiexec` para implementações OpenMPI.

A ativação de programas pode ocorrer nos modelos `SPMD` e `MPMD`
 
* Especificações dos hosts: `mpirun -n 3 -host h1,h2,h3 prog ...`
* Especificações dos arquivos: `mpirun -hostfile arg_hosts prog ...` ou `mpirun --machinefile arq_host prog ...`

Uma vez indicado os hosts, a atribuição de processos a estes nós pode ocorrer segundo 2 políticas: 
* `by slot`: política padrão, na qual MPI atribui processos aos nós até completarem-se os slots disponíveis 
* `by node`: selecionada com o parâmetro --bynode na ativação do programa,essa política faz com que MPI atribua processos a cada nó de cada vez, de forma circular entre os nós com slots disponíveis 

### Aspectos gerais da transmissão
Como mencionado anteriormente, MPI não necessita lidar necessariamente com o SOCK, dito isso:
* `int MPI_Send(void *buf, int count, MPI_Datatype dtype, int dest, int tag, MPI_Comm comm)`
* `int MPI_Recv(void *buf, int count, MPI_Datatype dtype, int src, int tag, MPI_Comm comm, MPI_Status *stat)`

Cada mensagem possui um atributo `tag` que representa um identificador de mensagem esperada.

### Tipo de dados a ser transmitido
* MPI_CHAR: signed char
* MPI_SHORT: signed short int
* MPI_INT: signed int
* MPI_LONG: signed long int
* MPI_UNSIGNED_CHAR: unsigned char
* MPI_UNSIGNED_SHORT: unsigned short int
* MPI_UNSIGNED: unsigned int
* MPI_UNSIGNED_LONG: unsigned long int
* MPI_FLOAT: float
* MPI_DOUBLE: double
* MPI_LONG_DOUBLE: long double
* MPI_BYTE: 8 binary digits
* MPI_PACKED: data packed or unpacked with MPI_Pack()/ MPI_Unpack

`MPI_PACKED` utilizado para empacotar dados a serem enviados 

### Tipo de transmissão ponto a ponto 
* Standard: nas transmissões padrão, não há sincronismo entre emissor e receptor. Se forem providos buffers, o envio pode ser concluído antes do recebimento. No caso de transmissão não bloqueantes, as operações `MPI_Wait()` e `MPI_Test()` podem ser utilizadas para saber se foram concluídas.
* Buffered: transmissões podem ser bufferizadas. Para tanto, as chamadas `MPI_Buffer_attach()` e `MPI_Buffer_detach` tratam da definição de espaços de buffer. Nesse tipo de transmissão bufferizada, o envio pode ser concluído antes do recebimento ser selecionado. 
* Synchronous: nas transmissões síncronas, as operações envio e recebimento podem ser realizadas em qualquer ordem, mas a transmissão só ocorre quando ambas as operações forem emitidas. Deste modo, além de prover a transmissão, essas chamadas servem para a sincronização entre as partes envolvidas. 
* Ready: nesse modo de transmissão, o envio pode ser iniciado apenas quando o recebimente já foi solicitado, ou um erro é resultado. 

As transmissões providas pela biblioteca MPI são extremamente confiáveis entretanto, cabe ao programador garantir que não ocorrerá `starvation` no recebimento de mensagem por processos concorrentes. Ou seja, se vários processos competem pelo recebimento de algum tipo de mensagem, MPI não garante que todos receberão mensaens.  

### Transmissões não bloqueantes 
De maneira geral, toda mensagem em MPI é bufferizada, mas de forma não visível. Caso não haja espeço no buffer, devido as transmissões anteriores ainda pendentes,a tarefa que emitiu a chamada é bloqueada. 

Já quando utilizamos a primitiva `MPI_Isend` a chamada retorna imediatamente, mesmo que os dados a transmitir não tenham sido copiados para o buffer da biblioteca. Esse é um parámetro não bloqueante. Um parâmetro extra presente nesta chamada, `request`, pode ser utilizado para saber o estado da operações. Da mesma forma, temos o `MPI_Irecv` que torna o recebimento não bloqueante. 

### MPI_ANY_SOURCE e MPI_ANY_TAG
Utilizado essas diretrizes para recebimento fora de ordem dessa forma, como não sabemos quem enviou a mensagem devemos utilizar a função `MPI_Get_count`

### MPI_Probe
Utilizamos essa diretiva quando não sabemos o tamanho da mensagem a ser recebida. Dessa forma, é alocado um buffer com o maior tamanho da mensagem prevista.

Entretanto, como as mensagens transmitidas via socket pela implementação MPI são armazenadas previamente em buffers locais, é possível à aplicação saber previamente o tamanho de uma messagem já recebida pela API, antes chamar a operação MPI_Recv.

Essa consulta pode ser feita com a chamada MPI_Probe:
* `MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status* status)`

De maneira equivalente ao MPI_Recv, MPI_Probe bloqueia à espera do recebimento de uma mensagem específica pela implementação MPI, sem que o conteúdo seja copiado para algum endereço especificado pela aplicação, contudo. 
Quando a chamada retorna, a estrutura status pode ser utilizada com a função `MPI_Get_count` para saber o número de itens recebido. É possível então alocar um buffer de tamanho apropriado para o recebimento. 

Caso a alocação e desalocação seja constante, vale apenas utilizar o tamanho de buffer pré definido. 

### Envio não bloqueante (imediato)
Quando utilizamos algo nesse sentido (pelo que entendi) não necessariamente precisamos definir um worker pois todas as tarefas irão receber e enviar mensagens.
Dessa forma também, podemos utilizar a função `MPI_Sendrecv(...)`

### Comunicação coletiva
Tipos de operações
* `Syncronization`: operações coletivas deste tipo servem para que processos esperem até que todos os membros do grupo atinjam um ponto de sincronização
* `Data movement`: permite a transferência de dados, em diferentes modelos (broadcast, scatter/gather, all to all, ...)
* `Collective Computation (reductions)`: são operações coletivas em que um membro do grupo coleta os dados e realiza uma operação sobre eles, como nas operações de redução (min, max, add, multiply, etc...)

Considerações:
* Como envolvem todos os processos em um grupo, operações coletivas são bloqueantes e têm efeito de sincronizar todos os processos do grupo
* Não há tags nas mensagens enviadas nessa operação
* Para realizar operações coletivas usando apenas sub-conjutos dos processos da aplicação, é preciso criar sub-grupos apropriados
* Apenas tipos pŕe definidos por MPI podem ser usados nas comunicações.

Primitivas:
* `MPI_Barrer`: cria uma barreira de sincronização
* `MPI_Bcast`: o processo cujo o rank foi especificado como root da chamada difunde a mensagem para os outros
* `MPI_Scatter`: o processo cujo o rank foi especificado como root difunde as mensagens distintas para os demais processos
* `MPI_Gather`: de forma contrária ao MPI_Scatter, o processo identificado como root coleta dos dados dos demais processos.
* `MPI_Allgather`: nessa operação todos os processos obtem todos os dados transmitidos por todos do grupo
* `MPI_Reduce`: processo root coleta os dados dos demais e realiza uma operação de agregação 
* `MPI_Allreduce`: realiza a operação de concatenação, porém todos os processos recebem o resultado
* `MPI_Reduce_scatter`:
* `MPI_Alltoall`:
* `MPI_Scan`:


## CUDA 
Do ponto de vista de Hardware, cuda é um aglomerado de processadores organizados de forma hierárquica.

De maneira geral, a programação com CUDA envolve as seguintes etapas:

* Declarar e alocar variáveis e espaços de memória para dados na RAM (*host memory*) (para as atividades executando em CPU);
* Declarar e alocar variáveis necessárias no espaço de endereçamento da GPU (*device memory*), para os processamentos em GPU;
* Inicializar dados no *host*;
* Transferir dados do *host* para o *device*;
  <br>[ ou Inicializar dados direteramente no *device*; ]
* Executar um ou mais *kernels* no *device*;
* Transferir os resultados da memória do *device* para a memória do *host*.

A declaração e ativação de códigos para GPU ocorrem dentro do próprio programa, onde caca código de GPU é declarado como uma função que tem prefixo `__global__` dessa forma nosso compilador sabe que é necessário gerar o código para GPU. 

Variáveis declaradas dentro de uma função de kernel serão alocadas automaticamente na área de memória do devide. 
Na ativação da função do kernel, especifica também a organização lógica da threads que serão utilizadas na execução desse código. 
A ativação do código de um kernel em GPU é feita de forma semelhante a uma chamada de função no programa da CPU. O uso dos caracteres `<<<,>>>` após o nome da função serve para indicar o número e a organização lógica dos processadores da GPU.

### Sincronismo de CPU e GPU
Após a execução de um kernel ser ativada, o fluxo de execução de instruções em CPU continua normalmente. Assim, CPUs e GPUs trabalham juntos de forma assíncrona.
Caso a operação em GPU seja necessária realizar antes da outra operação em CPU utilizamos `cudaDeviceSyncronize(*)`

### Tratamento de erros 
A maioria das funções da API CUDA, quando invocadas, tem como retorno um valor do tipo `cudaError_t`. Em caso de sucesso na execução, o valor retornado é `cudaSucess` e, em caso de falha, é possível detectar a falha utilizando a função `cudaGetErrorString()`

Já na ativação do kernel em GPU, não há valor de retorno. Entretanto, erros podem acontecer e é importante identificá-los. Isso pode ser feito utilizando a função `cudaGetLastError()`

Com o `cudaDeviceSyncronize` é possível obter erros que foram ocorridos durante a execução do programa.

### Blocos de threads 
A execução de instruções em GPU é feita no modelo SIMT, criando threads a partir de funções definidas como kernels.

* As threads são organizadas em blocos. A estrutura dos blocos é definida por uma variável do tipo `dim3`, que possibilita arranjos em 1,2 ou 3 dimensões.
* É possível definir um grid de blocos, que podem ser organizados em estruturas lógicas de 1,2 ou 3 dimensões. 

Assim, há 2 parâmetros que são passados na ativação de um kernel. O primeiro indica a estruturação do grid, ou seja, quantos blocos de threads serão usados e como esses blocos estarão organizados. O segundo parâmetro indica como as threads estarão organizadas dentro de cada bloco.

`kernel_function <<<grid, bloco >>>`

ou seja 

`kernel_function <<< #blocos, #threads_por_bloco>>>`

Durante a execução de uma das threads, há variáveis que permitem a cada uma das threads saber quais são as dimensões do bloco que ela pertence e qual é o posicionamento dela dentro do bloco, nas dimensões X,Y e Z.
* Thread dentro do bloco: `threadIdx.x`, `threadIdx.y`, `threadIdx.z`
* Bloco dentro do grid: `blockIdx.x`, `blockIdx.y`, `blockIdx.z`
* Dimensões do bloco: `blockDim.x`, `blockDim.y`, `blockDim.z`
* Dimensões do grid em blocos: `grimDim.x`, `grimDim.y`, `grimDim.z`

As variáveis `threadIdx` e `blockIdx` indicam a posição lógica de cada thread na estrutura.