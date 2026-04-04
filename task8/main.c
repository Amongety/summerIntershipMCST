#define __USE_MISC
#define __USE_POSIX
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define SIZE_MAT 100

struct sigaction orig_sigint_handler;
int i = 0, j = 0, k = 0;

void randomFill(int [SIZE_MAT][SIZE_MAT]);
void signalSigint(int);
void actionSigint(int);
void multi_matrix(int [SIZE_MAT][SIZE_MAT], const int [SIZE_MAT][SIZE_MAT], const int [SIZE_MAT][SIZE_MAT]);

int main(int argc, char *argv[]) {
    int matrix1[SIZE_MAT][SIZE_MAT], matrix2[SIZE_MAT][SIZE_MAT], res[SIZE_MAT][SIZE_MAT];

    if(argc != 2) {
        printf("Invalid arguments\n");
        return -1;
    }

    srand(time(NULL));
    randomFill(matrix1);
    randomFill(matrix2);

    if(!strcmp(argv[1], "--signal")) signal(SIGINT, signalSigint);
    else if(!strcmp(argv[1], "--sigaction")) {
        struct sigaction act;
        memset((void*)&act, 0, sizeof(act));
        act.sa_handler = actionSigint;
        act.sa_flags = SA_RESTART;
        sigaction(SIGINT, &act, &orig_sigint_handler);
    }

    multi_matrix(res, matrix1, matrix2);

    return 0;
}

void randomFill(int mat[SIZE_MAT][SIZE_MAT]) {
    for(int i = 0; i < SIZE_MAT; ++i) {
        for(int j = 0; j < SIZE_MAT; ++j) {
            mat[i][j] =  rand() % (SHRT_MAX - SHRT_MIN + 1) + SHRT_MIN;
        }
    }
}

void signalSigint(int s) {
    printf("\n%d %d\n%d %d\n%d %d\n", i, k, k, j, i, j);
    
    signal(SIGINT, SIG_DFL);
}


void actionSigint(int signo) {
    printf("\n%d %d\n%d %d\n%d %d\n", i, k, k, j, i, j);

    sigaction(SIGINT, &orig_sigint_handler, NULL);
}

void multi_matrix(int res[SIZE_MAT][SIZE_MAT], const int mat1[SIZE_MAT][SIZE_MAT], const int mat2[SIZE_MAT][SIZE_MAT]) {
    for(; i < SIZE_MAT; ++i) {
        j = 0;
        for(; j < SIZE_MAT; ++j) {
            res[i][j] = 0;
            k = 0;
            for (; k < SIZE_MAT; ++k) res[i][j] += mat1[i][k] * mat2[k][j];
            
            usleep(500000);
        }
    }
}