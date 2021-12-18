#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int N = atoi(argv[1]);
    char *outfile_name = argv[2];

    FILE *outfile = fopen(outfile_name, "w");
    fprintf(outfile, "%i\n", N);
    for (int i = 0; i < 2*N; ++i) {
        if (i < N) {
            for (int j = 0; j < N; ++j) {
                fprintf(outfile, "%i ", rand() % 10);
            }
            fprintf(outfile, "\n");
        } else {
            for (int j = 0; j < N; ++j) {
                fprintf(outfile, "%i ", rand() % 10);
            }
            fprintf(outfile, "\n");    
        }
    }

    fclose(outfile);
}