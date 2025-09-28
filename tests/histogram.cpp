//
// Created by os on 9/27/25.
//

#include "histogram.h"
#include "printing.hpp"
#include "../h/TCB.hpp"

#define MOD 10

static unsigned long int next = 1;

int custom_rand(void) {
    next = next * 1103515245 + 12345;
    return (unsigned) (next / 65536) % 32768;
}

void custom_srand(unsigned int seed) {
    next = seed;
}


static int M;
static int N;

static int** matrix;
int _histogram[MOD] = {0};
sem_t mutex;

void worker(void* arg) {

    int row = *(int *)arg;
    int localHistogram[MOD] = {0};

    for (int i = 0; i < N; i++) {
        int value = matrix[row][i] % MOD;
        localHistogram[value]++;

        if ((i+1) % MOD == 0) {
            time_sleep(5);

        }
    }

    sem_wait(mutex);

    for (int j = 0; j < MOD; j++) {
        _histogram[j] += localHistogram[j];
    }

    sem_signal(mutex);

}

void histogram() {

    char input[3];

    printString("Enter matrix height: ");
    getString(input, 3);
    M = stringToInt(input);
    printString("\n");

    printString("Matrix height is: ");
    printInt(M);
    printString("\n");


    printString("Enter matrix  width: ");
    getString(input, 3);
    N = stringToInt(input);
    printString("\n");

    printString("Matrix width is: ");
    printInt(N);
    printString("\n");

    matrix = (int**)mem_alloc(sizeof(int*) * M);
    for (int i = 0; i < M; i++) {
        matrix[i] = (int*)mem_alloc(sizeof(int) * N);
        for (int j = 0; j < N; j++) {
            matrix[i][j] = custom_rand();
            printInt(matrix[i][j]);
            printString(" ");
        }
        printString("\n");
    }
    printString("Matrix is initilized with pseudo random numbers\n");

    thread_t threads[M];
    int indexes[M];
    sem_open(&mutex, 1);

    for (int i = 0; i < M; i++) {
        indexes[i] = i;
        thread_create(&threads[i], worker, (void*)&indexes[i]);
        thread_start(threads[i]);
    }

    for (int i = 0; i < M; i++) {
        thread_join(&threads[i]);
    }

    printString("All threads are finished\n");

    printString("Histogram: ");

    for (int i = 0; i < MOD; i++) {
        printInt(_histogram[i]);
        printString(" ");
    }
    printString("\n");


    for (int i =0; i < M; i++) delete matrix[i];
    delete matrix;
    sem_close(mutex);

}
