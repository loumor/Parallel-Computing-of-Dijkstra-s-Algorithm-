#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <omp.h>

#include "Dijkstra_tools.h"

long N;
int randInit;

void inputCheck(int argc, char** argv);
void printUsage(int argc, char** argv);
void dijkstra(graph* G, long initial_node, char debug);
void testScheduler(int nThreads, graph* G);


int main(int argc, char *argv[])
{
	long i;
	graph G;
    double runtime;
    double runtime_graph;
    struct timeval start;
    struct timeval end;
    struct timeval start_graph;
    struct timeval end_graph;
    gettimeofday(&start, NULL);
    
	inputCheck(argc, argv);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    int nthreads = 4; // USER SET NUMBER OF THREADS
    
    
    
    
    
    
	if(N == 1){
		generateTestGraph(&G);
	} else {
        gettimeofday(&start_graph, NULL);
        
		generateGraph(N, randInit, &G, 0, nthreads);
        
        if( gettimeofday(&end_graph, NULL) != 0){
            printf("Getting time failed!\n");
        }
        runtime_graph = (double) end_graph.tv_sec-start_graph.tv_sec + ( end_graph.tv_usec - start_graph.tv_usec ) / 1000000.0;
	}

    printf("\nTesting with max %d Threads\n", nthreads);
    testScheduler(nthreads,&G);

    if( gettimeofday(&end, NULL) != 0){
        printf("Getting time failed!\n");
    }
    runtime = (double) end.tv_sec-start.tv_sec + ( end.tv_usec - start.tv_usec ) / 1000000.0;
    
    printf("Was working for [%f] sec to run entire program.\n",runtime);
    
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

void testScheduler(int nThreads, graph* G)
{
	double runtime;

	//Set max nThreads
	omp_set_num_threads(nThreads);

	resetGraph(G);
	omp_set_schedule(omp_sched_static, G->N/nThreads);
	tick();
		dijkstra(G, 0, 1);
	runtime = tack();
	printf("Was working for [%f] sec to run Dijkstra.\n",runtime);
}

void dijkstra(graph* G, long initial_node, char debug)
{
	long i,j,k;
	long aN; //actualNode
	G->D[initial_node] = 0;
	aN = initial_node;

 /* Graphing Check */
//    if(debug){
//        printf("Running dijkstra on graph\n");
//        printGraph(G);
//    }
    
    /* Grainuality Control */
    
    if (G-N > 500){  // Exploit Parallelism after 500 nodes
        for(i = 0; i < G->N; i++)
        {
            G->visited[aN] = VISITED;
            
              /* Graphing Check */
            //        if(debug){
            //            printf("It[%d] aN [%d]",i, aN); printStatus(G); printf("\n");
            //        }
            
            //Find all nodes connected to aN
            #pragma omp parallel for schedule(runtime)
            for(j=0;j<G->N;j++){
                if( (G->node[aN][j] != NO_CONN) ){
                    if( (G->D[aN] + G->node[aN][j]) < G->D[j] ){
                        G->D[j] = (G->D[aN] + G->node[aN][j]);
                    }
                }
            }
            aN = getNextNode(G);
        }
    }
    else{
        for(i = 0; i < G->N; i++)
        {
            G->visited[aN] = VISITED;
            
             /* Graphing Check */
            //        if(debug){
            //            printf("It[%d] aN [%d]",i, aN); printStatus(G); printf("\n");
            //        }
            
            //Find all nodes connected to aN
            for(j=0;j<G->N;j++){
                if( (G->node[aN][j] != NO_CONN) ){
                    if( (G->D[aN] + G->node[aN][j]) < G->D[j] ){
                        G->D[j] = (G->D[aN] + G->node[aN][j]);
                    }
                }
            }
            aN = getNextNode(G);
        }
    }
}

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
