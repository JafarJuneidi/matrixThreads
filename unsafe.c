#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
int **M1;
int **M2;

void input_handler (char *infile_name) {
	FILE *infile = fopen(infile_name, "r");

	char *line = NULL;
	size_t _ = 0;
	ssize_t read;
	int N;
	getline(&line, &_, infile);
	N = (int)(line[0] - '0');

	// int m1[N][N], m2[N][N];
	M1 = (int **)malloc(N * sizeof(int *)); 
	M2= (int **)malloc(N * sizeof(int *));

	for(int i = 0; getline(&line, &_, infile) != -1; ++i) {
		if (i < N) {
			M1[i] = (int *)malloc(N * sizeof(int));
			for (int j = 0; j < N; ++j) {
				M1[i][j] = (int)(line[2*j] - '0');
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

int main (int argc, char *argv[]) {
	int NUM_THREADS = atoi(argv[1]);
	char *outfile_name = argv[2];

	char *infile_name = argv[3];
	input_handler(infile_name);
	
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
	
}
