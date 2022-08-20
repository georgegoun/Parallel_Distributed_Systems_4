// The algorithm is based on a very simple idea: select a point as the vantage point,
// compute the distance of every point to the vantage
// point and split the points according to the median distance.

#include "../inc/functions/linked_list/insert_node.h"
#include "../inc/functions/median.h"
#include "../inc/helpers/eucDist.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10
#define d 2
#define high 100.0
#define low 0.0

int main(int argc, char* argv[])
{
    time_t t;

    double** data = (double**)malloc(N * sizeof(double*));

    for (int i = 0; i < N; i++) {
        data[i] = (double*)malloc(d * sizeof(double));
    }

    srand((unsigned)time(&t));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < d; j++) {
            data[i][j] = ((double)rand() * (high - low)) / (double)RAND_MAX + low;
            printf("%.2f ", data[i][j]);
        }
        printf("\t");
        if ((i + 1) % 2 == 0) {
            printf("\n");
        }
    }
    printf("\n\n");
    double* distances = malloc(N * sizeof(double));

    int id_vp = rand() % N;
    for (int i = 0; i < N; i++) {
        distances[i] = eucDist(data[id_vp], data[i], d);
        printf("%.2f\t", distances[i]);
        if ((i + 1) % 2 == 0) {
            printf("\n");
        }
    }
    printf("\n\n");
    printf("median is: %lf\n", median(distances, N, (N / 2) + 1));
    for (int i = 0; i < N; i++) {
        printf("%.2f\t", distances[i]);
        if ((i + 1) % 2 == 0) {
            printf("\n");
        }
    }

    create_node(data, id_vp, data[id_vp], N);
}