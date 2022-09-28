// The algorithm is based on a very simple idea: select a point as the vantage point,
// compute the distance of every point to the vantage
// point and split the points according to the median distance.
#include "../inc/helpers/eucDist.h"
#include "../inc/helpers/timer.h"
#include "../inc/helpers/tree_info_calc.h"
#include "../inc/seq_functions/linked_list/insert_node.h"
#include "../inc/seq_functions/median.h"

// threads

#include "../inc/threads_functions/linked_list/insert_node_threads.h"
#include <pthread.h>

// knn search
#include "../inc/knn/knn_search.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10
#define d 2
#define high 100.0
#define low 0.0
#define NUMOFTHREADS 30
#define THRESHOLD 10000
#define k 2

int main(int argc, char* argv[])
{
    time_t t;

    // timers
    struct timespec start_seq = { 0 }, stop_seq = { 0 };
    struct timespec start_pthreads = { 0 }, stop_pthreads = { 0 };
    struct timespec start_threshold = { 0 }, stop_threshold = { 0 };

    int method_code = atoi(argv[1]);

    double** data = (double**)malloc(N * sizeof(double*));

    for (int i = 0; i < N; i++) {
        data[i] = (double*)malloc(d * sizeof(double));
    }

    srand((unsigned)time(&t));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < d; j++) {
            data[i][j] = ((double)rand() * (high - low)) / (double)RAND_MAX + low;
        }
    }

    int id_vp = rand() % N;

    // creating nodes

    int node_count = 1;
    int* node_count_ptr = &node_count;

    struct node** nodes = (struct node**)malloc(tree_info_calc(N) * sizeof(struct node*));
    nodes[0] = create_node(data, id_vp, data[id_vp], N, d);
    switch (method_code) {
        /*              Sequential              */
    case 1:
        // print_info(nodes[0], d);
        start_seq = timerStart(start_seq);
        for (int i = 0; i < *(node_count_ptr); i++) {
            vp_tree(nodes[i], nodes, node_count_ptr, d);
        }
        stop_seq = timerStop(stop_seq);
        printf("\nSequential %d elements in %d dimensions and %d nodes_created in %lf seconds\n", N, d, *node_count_ptr, timeDif(start_seq, stop_seq));
        break;

        /*              Pthreads               */
    case 2:
        start_pthreads = timerStart(start_pthreads);
        for (int i = 0; i < *(node_count_ptr); i++) {
            vp_tree_threads(nodes[i], nodes, node_count_ptr, d, NUMOFTHREADS);
        }
        stop_pthreads = timerStop(stop_pthreads);
        printf("\nPthreads %d elements in %d dimensions and %d nodes_created in %lf seconds\n", N, d, *node_count_ptr, timeDif(start_pthreads, stop_pthreads));
        break;
        /*              Threshold              */
    case 3:
        start_threshold = timerStart(start_threshold);
        for (int i = 0; i < *(node_count_ptr); i++) {
            if (nodes[i]->data_size < THRESHOLD) {
                vp_tree(nodes[i], nodes, node_count_ptr, d);
            } else {
                vp_tree_threads(nodes[i], nodes, node_count_ptr, d, NUMOFTHREADS);
            }
        }
        stop_threshold = timerStop(stop_threshold);
        printf("\nThreshold %d elements in %d dimensions and %d nodes_created in %lf seconds\n", N, d, *node_count_ptr, timeDif(start_threshold, stop_threshold));

        break;
    }
    if (atoi(argv[2]) == 1) {
        double*** knn = (double***)malloc(N * sizeof(double**));
        for (int i = 0; i < N; i++) {
            knn[i] = (double**)malloc(k * sizeof(double*));
            for (int j = 0; j < k; j++) {
                knn[i][j] = (double*)malloc(d * sizeof(double));
            }
        }
        for (int i = 0; i < N; i++) {
            knn_search(nodes, d, knn[i], k, i);
        }
        printf("data:\n");

        for (int j = 0; j < N; j++) {
            for (int l = 0; l < d; l++) {
                printf("%lf ", data[j][l]);
            }
            printf("\n");
        }
        printf("\nknn data search for: ");
        for (int i = 0; i < d; i++) {
            printf("%lf ", data[7][i]);
        }
        printf("\n");
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < d; j++) {
                printf("%lf ", knn[7][i][j]);
            }
            printf("\n");
        }
    }

    return 0;
}
