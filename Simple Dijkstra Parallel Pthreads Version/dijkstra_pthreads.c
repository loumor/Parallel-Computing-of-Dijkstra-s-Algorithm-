#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <pthread.h>

#include "Dijkstra_tools.h"

long N;
int randInit;
long aN; //actualNode
int nThreads;

void inputCheck(int argc, char** argv);
void printUsage(int argc, char** argv);
void dijkstra(graph* G, long initial_node, char debug);
void testScheduler(graph* G);

void * pthreadLoop_1_thread(void * G);
void * pthreadLoop_2_thread_1(void * G);
void * pthreadLoop_2_thread_2(void * G);
void * pthreadLoop_3_thread_1(void * G);
void * pthreadLoop_3_thread_2(void * G);
void * pthreadLoop_3_thread_3(void * G);
void * pthreadLoop1(void * G);
void * pthreadLoop2(void * G);
void * pthreadLoop3(void * G);
void * pthreadLoop4(void * G);


int main(int argc, char *argv[])
{
	long i;
	graph G;

	inputCheck(argc, argv);

	if(N == 1){
		generateTestGraph(&G);
	} else {
		generateGraph(N, randInit, &G, 0);
	}
    
    

    nThreads = 4; // USER CHOOSE NUMBER OF THREADS
    
    
    
    
    
    
    
    printf("\nTesting with max %d Threads\n", nThreads);
    testScheduler(&G);
    
      /* Graphing Check */
//    char *b;
//    b = malloc(G.N * 5);
//    if(b == NULL) {perror("malloc"); exit(EXIT_FAILURE); }
//    sprintf(b,"\nLowest distances!\nD=[");
//    for(i = 0; i<G.N; i++){
//        sprintf(&b[strlen(b)], "%d,", G.D[i]);
//    }
//    printf("%s]\n", b);
    
    return EXIT_SUCCESS;
}

void testScheduler(graph* G)
{
	double runtime;

	//Set max nThreads
	resetGraph(G);
	tick();
		dijkstra(G, 0, 1);
	runtime = tack();
	printf("Dijkstra was working for [%f] sec.\n",runtime);
}

void dijkstra(graph* G, long initial_node, char debug)
{
	long i,j,k;
	G->D[initial_node] = 0;
	aN = initial_node;
    pthread_t threads [nThreads]; // Create threads

	for(i = 0; i < G->N; i++)
	{
		G->visited[aN] = VISITED;

		//Find all nodes connected to aN
        /* Launch Threads */
        if (nThreads = 1){
            pthread_create(&threads[0], NULL, pthreadLoop_1_thread, (void*) G);
        }
        if (nThreads = 2){
            pthread_create(&threads[0], NULL, pthreadLoop_2_thread_1, (void*) G);
            pthread_create(&threads[1], NULL, pthreadLoop_2_thread_1, (void*) G);
        }
        if (nThreads = 3){
            pthread_create(&threads[0], NULL, pthreadLoop_3_thread_1, (void*) G);
            pthread_create(&threads[1], NULL, pthreadLoop_3_thread_2, (void*) G);
            pthread_create(&threads[2], NULL, pthreadLoop_3_thread_3, (void*) G);
        }
        if (nThreads = 4){
            pthread_create(&threads[0], NULL, pthreadLoop1, (void*) G);
            pthread_create(&threads[1], NULL, pthreadLoop2, (void*) G);
            pthread_create(&threads[2], NULL, pthreadLoop3, (void*) G);
            pthread_create(&threads[3], NULL, pthreadLoop4, (void*) G);
        }
        
        /* Wait for Threads to Finish */
        if (nThreads = 1){
            pthread_join(threads[0], NULL);
        }
        if (nThreads = 2){
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
        }
        if (nThreads = 3){
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
            pthread_join(threads[2], NULL);
        }
        if (nThreads = 4){
            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);
            pthread_join(threads[2], NULL);
            pthread_join(threads[3], NULL);
        }
		aN = getNextNode(G);
	}
	//printf("Finished Dijkstra\n");
}

///////////////////////////////////// THREAD BASED LOOP DISTRIBUTION OF WORK /////////////////

/* For one thread */
void * pthreadLoop_1_thread(void * G){
    graph *Gg = (graph *)G;
    for(long j=0;j<Gg->N;j++){
        if( (Gg->node[aN][j] != NO_CONN) ){
            if( (Gg->D[aN] + Gg->node[aN][j]) < Gg->D[j] ){
                Gg->D[j] = (Gg->D[aN] + Gg->node[aN][j]);
            }
        }
    }
    //printf("Threads loop 1 done\n");
}

/* For two threads part 1 */

void * pthreadLoop_2_thread_1(void * G){
    graph *Gg = (graph *)G;
    for(long j=0;j<Gg->N/2;j++){
        if( (Gg->node[aN][j] != NO_CONN) ){
            if( (Gg->D[aN] + Gg->node[aN][j]) < Gg->D[j] ){
                Gg->D[j] = (Gg->D[aN] + Gg->node[aN][j]);
            }
        }
    }
    //printf("Threads loop 1 done\n");
}

/* For two threads part 2 */

void * pthreadLoop_2_thread_2(void * G){
    graph *Gg = (graph *)G;
    for(long j=Gg->N/2;j<Gg->N;j++){
        if( (Gg->node[aN][j] != NO_CONN) ){
            if( (Gg->D[aN] + Gg->node[aN][j]) < Gg->D[j] ){
                Gg->D[j] = (Gg->D[aN] + Gg->node[aN][j]);
            }
        }
    }
    //printf("Threads loop 1 done\n");
}

/* For three threads part 1 */

void * pthreadLoop_3_thread_1(void * G){
    graph *Gg = (graph *)G;
    for(long j=0;j<Gg->N/3;j++){
        if( (Gg->node[aN][j] != NO_CONN) ){
            if( (Gg->D[aN] + Gg->node[aN][j]) < Gg->D[j] ){
                Gg->D[j] = (Gg->D[aN] + Gg->node[aN][j]);
            }
        }
    }
    //printf("Threads loop 1 done\n");
}

/* For three threads part 2 */

void * pthreadLoop_3_thread_2(void * G){
    graph *Gg = (graph *)G;
    for(long j=Gg->N/3;j<(2 * (Gg->N/3));j++){
        if( (Gg->node[aN][j] != NO_CONN) ){
            if( (Gg->D[aN] + Gg->node[aN][j]) < Gg->D[j] ){
                Gg->D[j] = (Gg->D[aN] + Gg->node[aN][j]);
            }
        }
    }
    //printf("Threads loop 1 done\n");
}

/* For three threads part 3 */

void * pthreadLoop_3_thread_3(void * G){
    graph *Gg = (graph *)G;
    for(long j=(2 * (Gg->N/3));j<Gg->N;j++){
        if( (Gg->node[aN][j] != NO_CONN) ){
            if( (Gg->D[aN] + Gg->node[aN][j]) < Gg->D[j] ){
                Gg->D[j] = (Gg->D[aN] + Gg->node[aN][j]);
            }
        }
    }
    //printf("Threads loop 1 done\n");
}


/* For four threads part 1 */
void * pthreadLoop1(void * G){
   graph *Gg = (graph *)G;
        for(long j=0;j<Gg->N/4;j++){
            if( (Gg->node[aN][j] != NO_CONN) ){
                if( (Gg->D[aN] + Gg->node[aN][j]) < Gg->D[j] ){
                    Gg->D[j] = (Gg->D[aN] + Gg->node[aN][j]);
                }
            }
        }
}

/* For four threads part 2 */
void * pthreadLoop2(void * G){
    graph *Gg = (graph *)G;
    for(long j=Gg->N/4;j<Gg->N/2;j++){
        if( (Gg->node[aN][j] != NO_CONN) ){
            if( (Gg->D[aN] + Gg->node[aN][j]) < Gg->D[j] ){
                Gg->D[j] = (Gg->D[aN] + Gg->node[aN][j]);
            }
        }
    }
}

/* For four threads part 3 */
void * pthreadLoop3(void * G){
    graph *Gg = (graph *)G;
    for(long j=Gg->N/2;j<(Gg->N/2+Gg->N/4);j++){
        if( (Gg->node[aN][j] != NO_CONN) ){
            if( (Gg->D[aN] + Gg->node[aN][j]) < Gg->D[j] ){
                Gg->D[j] = (Gg->D[aN] + Gg->node[aN][j]);
            }
        }
    }
}

/* For four threads part 4 */
void * pthreadLoop4(void * G){
    graph *Gg = (graph *)G;
    for(long j=Gg->N/4;j<Gg->N;j++){
        if( (Gg->node[aN][j] != NO_CONN) ){
            if( (Gg->D[aN] + Gg->node[aN][j]) < Gg->D[j] ){
                Gg->D[j] = (Gg->D[aN] + Gg->node[aN][j]);
            }
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////




void printUsage(int argc, char** argv){
	printf("Usage: %s NUMER_OF_POINTS [SRAND_INIT_VALUE]\n",argv[0]);
}

void inputCheck(int argc, char** argv)
{
	if ( argc < 2){
		printUsage(argc, argv);
		exit(EXIT_FAILURE);
	}

	errno = 0; // To distinguish success/failure after call
	N = atol(argv[1]);

	/* Check for various possible errors */

	if ((errno == ERANGE && (N == LONG_MAX || N == LONG_MIN)) || (errno != 0 && N == 0)) {
	   perror("atol");
	   exit(EXIT_FAILURE);
	}

	if(N <= 0){
		printf("Invalid number of points! Number of points must be bigger than zero.\n");
		exit(EXIT_FAILURE);
	}

	//Check for second argument being the random init value
	if(argc == 3)
	{
		errno = 0; // To distinguish success/failure after call
		randInit = atol(argv[2]);

		/* Check for various possible errors */

		if ((errno == ERANGE && (randInit == LONG_MAX || randInit == LONG_MIN)) || (errno != 0 && randInit == 0)) {
		   perror("atol");
		   exit(EXIT_FAILURE);
		}
	} else {
		randInit = -1;
	}

	if(randInit < 0){
		time_t t1;
		struct tm* t2 = NULL;
		int sec;
		t1 = time( NULL );
		t2 = gmtime(&t1);
		sec = t2->tm_sec;
		randInit = sec;
	}
}
