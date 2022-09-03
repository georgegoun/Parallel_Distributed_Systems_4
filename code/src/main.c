// The algorithm is based on a very simple idea: select a point as the vantage point,
// compute the distance of every point to the vantage
// point and split the points according to the median distance.
#include "../inc/helpers/eucDist.h"
#include "../inc/helpers/tree_info_calc.h"
#include "../inc/seq_functions/linked_list/insert_node.h"
#include "../inc/seq_functions/median.h"

// threads
#include "../inc/threads_functions/linked_list/insert_node_threads.h"
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 10
#define d 2
#define high 100.0
#define low 0.0
#define NUMOFTHREADS 3

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
        }
    }

    int id_vp = rand() % N;

    // creating nodes

    int node_count = 1;
    int* node_count_ptr = &node_count;

    struct node** nodes = (struct node**)malloc(tree_info_calc(N) * sizeof(struct node*));

    nodes[0] = create_node(data, id_vp, data[id_vp], N, d);

    /*
        // print_info(nodes[0], d);
        for (int i = 0; i < *(node_count_ptr); i++) {
            vp_tree(nodes[i], nodes, node_count_ptr, d);
            printf("node count is: %d\n", *node_count_ptr);
            // print_info(nodes[0], d);
        }

        for (int i = 0; i < *node_count_ptr; i++) {
            print_info(nodes[i], d);
        }
    */

    // threads
    for (int i = 0; i < *(node_count_ptr); i++) {
        vp_tree_threads(nodes[i], nodes, node_count_ptr, d, NUMOFTHREADS);
        printf("node count is: %d\n", *node_count_ptr);
    }

    for (int i = 0; i < *node_count_ptr; i++) {
        print_info(nodes[i], d);
    }
    return 0;
}

// TODO: REMOVE DEFINE N AND d FROM THE INSERT_NODE.H FILE  \//
//       MAKE BRAKE CONDITION FOR THE VP_TREE FUNCTION      \//
//       CLEAR A BIT THE MAIN FUNCTION                      \//