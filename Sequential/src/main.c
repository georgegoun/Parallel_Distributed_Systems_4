// The algorithm is based on a very simple idea: select a point as the vantage point,
// compute the distance of every point to the vantage
// point and split the points according to the median distance.

#include "../include/functions/median.h"
#include "../include/helpers/eucDist.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 100
#define d 3

int main(int argc, char* argv[])
{
    time_t t;

    int** randomArray = (int**)malloc(N * sizeof(int*));

    for (int i = 0; i < N; i++) {
        randomArray[i] = (int*)malloc(d * sizeof(int));
    }

    srand((unsigned)time(&t));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < d; j++) {
            randomArray[i][j] = rand() % 10;
            printf("%d ", randomArray[i][j]);
        }
        printf("\t");
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }

    int* distances = malloc(N * sizeof(double));

    int idx = rand() % 100;
    for (int i = 0; i < N; i++) {
        distances[i] = eucDist(randomArray[idx], randomArray[i], d);
        printf("%d\t", distances[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    printf("median is: %lf\n", median(distances, N, (N / 2) + 1));
    for (int i = 0; i < N; i++) {
        printf("%d ", distances[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
}