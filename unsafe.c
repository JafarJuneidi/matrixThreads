#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
int TOTAL_NUM_THREADS;
int NUM_THREADS;
int N;
int **M1;
int **M2;

int numOfEven = 0;
int numOfOdd = 0;
int totalNum = 0;

int input_handler(const char* infile_name)
{
    FILE *infile = fopen(infile_name, "r");
    if (infile == NULL) return 0;

	fscanf (infile, "%d", &N);
	printf("%i\n", N);

	M1 = (int **)malloc(N * sizeof(int *)); 
	M2 = (int **)malloc(N * sizeof(int *));

    for(size_t i = 0; i < 2*N; ++i)
    {
		if (i < N) {
			M1[i] = (int *)malloc(N * sizeof(int));
        	for(size_t j = 0; j < N; ++j)
            	fscanf(infile, "%d", *(M1+i)+j);
    	} else {
			M2[i%N] = (int *)malloc(N * sizeof(int));
			for(size_t j = 0; j < N; ++j)
            	fscanf(infile, "%d", *(M2+i%N)+j);
		}
	}

    fclose (infile); 
    return 1; 
}

void *calculate(void *threadid) {
	int id = (intptr_t)threadid;
	if (id >= N*N) {
		printf("thread %i is not working\n", id);
		return NULL;
	}
	printf("thread %i is running\n", id);
	int elemPerThread = N*N / NUM_THREADS;
	int startElem = id * elemPerThread;
	if (id == NUM_THREADS - 1) {
		elemPerThread = N*N - startElem;
	}
	
	printf("threadID = %i, startLoop = %i, endLoop = %i\n", id, startElem, startElem + elemPerThread);
	for (int i = startElem; i < startElem + elemPerThread; ++i) {
		int row = i / N;
		int col = i % N;
		int result = 0;
		for (int j = 0; j < N; ++j) {
			result += M1[row][j] * M2[j][col];
		}
		if (result % 2 == 0) {
			numOfEven++;
		} else {
			numOfOdd++;
		}
		totalNum++;
	}
	
	pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
	TOTAL_NUM_THREADS = atoi(argv[1]);
	char *outfile_name = argv[2];

	char *infile_name = argv[3];
	input_handler(infile_name);

	if (TOTAL_NUM_THREADS > N*N) {
		NUM_THREADS = N*N;
	} else {
		NUM_THREADS = TOTAL_NUM_THREADS;
	}

	pthread_t threads[TOTAL_NUM_THREADS];
	int rc, t;
	for (t=0; t<TOTAL_NUM_THREADS; t++) {
		printf("Creating thread %d\n", t);
		rc = pthread_create(&threads[t], NULL, calculate, (void *) (intptr_t) t);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	for (t=0; t<TOTAL_NUM_THREADS; t++) {
		pthread_join(threads[t], NULL);
	}

	FILE *outfile = fopen(outfile_name, "w");
	fprintf(outfile, "numOfEven: %i, numOfOdd: %i, totalCells: %i\n", numOfEven, numOfOdd, totalNum);
	printf("numOfEven: %i, numOfOdd: %i, totalCells: %i\n", numOfEven, numOfOdd, totalNum);

	pthread_exit(NULL);
}
