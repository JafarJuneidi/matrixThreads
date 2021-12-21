#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
int TOTAL_NUM_THREADS;
int NUM_THREADS;
int N;
int **M1;
int **M2;
// int **M3; 
pthread_mutex_t lock;

int numOfEven = 0;
int numOfOdd = 0;
int totalNum = 0;

void input_handler (char *infile_name) {
	FILE *infile = fopen(infile_name, "r");

	char *line = NULL;
	size_t _ = 0;
	ssize_t read;
	getline(&line, &_, infile);
	N = (int)(line[0] - '0');

	// int m1[N][N], m2[N][N];
	M1 = (int **)malloc(N * sizeof(int *)); 
	M2 = (int **)malloc(N * sizeof(int *));
	// M3 = (int **)malloc(N * sizeof(int *));

	for(int i = 0; getline(&line, &_, infile) != -1; ++i) {
		if (i < N) {
			// M3[i] = (int *)malloc(N * sizeof(int));
			M1[i] = (int *)malloc(N * sizeof(int));
			for (int j = 0; j < N; ++j) {
				M1[i][j] = (int)(line[2*j] - '0');
				// M3[i][j] = 0;
			}	
		} else {
			M2[i%N] = (int *)malloc(N * sizeof(int));
			for (int j = 0; j < N; ++j) {
				M2[i%N][j] = (int)(line[2*j] - '0');
			}
		}
	}

	fclose(infile);
}

void *calculate(void *threadid) {
	int id = (intptr_t)threadid;
	if (id >= N*N) {
		printf("thread %i is not working\n", id);
		return NULL;
	}
	// printf("thread %i is running\n", id);
	int elemPerThread = N*N / NUM_THREADS;
	int startElem = id * elemPerThread;
	if (id == NUM_THREADS - 1) {
		elemPerThread = N*N - startElem;
	}

	printf("thread %i : calc %i elements : starting from element %i\n", id, elemPerThread, startElem);
	// pthread_mutex_lock(&lock);
	for (int i = startElem; i < startElem + elemPerThread; ++i) {
		int row = i / N;
		int col = i % N;
		int result = 0;
		for (int j = 0; j < N; ++j) {
			result += M1[row][j] * M2[j][col];
		}
		// M3[row][col] = result;
		if (result % 2 == 0) {
			// pthread_mutex_lock(&lock);
			numOfEven++;
		} else {
			// pthread_mutex_lock(&lock);
			numOfOdd++;
		}
		totalNum++;
		// pthread_mutex_unlock(&lock);
	}
	// pthread_mutex_unlock(&lock);
	
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
	
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j){
			printf("%i", M1[i][j]);
		}
		printf("\n");
	}

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j){
			printf("%i", M2[i][j]);
		}
		printf("\n");
	}

	// for (int i = 0; i < 4; ++i) {
	// 	for (int j = 0; j < 4; ++j){
	// 		printf("%i", M3[i][j]);
	// 	}
	// 	printf("\n");
	// }

	pthread_t threads[TOTAL_NUM_THREADS];
	int rc, t;
	for (t=0; t<TOTAL_NUM_THREADS; t++) {
		// printf("Creating thread %d\n", t);
		rc = pthread_create(&threads[t], NULL, calculate, (void *) (intptr_t) t);
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	for (t=0; t<TOTAL_NUM_THREADS; t++) {
		pthread_join(threads[t], NULL);
	}

	printf("numOfEven: %i, numOfOdd: %i, totalCells: %i\n", numOfEven, numOfOdd, totalNum);

	// for (int i = 0; i < 4; ++i) {
	// 	for (int j = 0; j < 4; ++j){
	// 		printf("%i", M3[i][j]);
	// 	}
	// 	printf("\n");
	// }

	// pthread_exit(NULL);
}
