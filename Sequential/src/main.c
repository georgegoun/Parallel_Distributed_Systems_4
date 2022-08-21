// The algorithm is based on a very simple idea: select a point as the vantage point,
// compute the distance of every point to the vantage
// point and split the points according to the median distance.

#include "../inc/functions/linked_list/insert_node.h"
#include "../inc/functions/median.h"
#include "../inc/helpers/eucDist.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 20
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
        }
    }

    int id_vp = rand() % N;

    // creating nodes

    int node_count = 1;
    int* node_count_ptr = &node_count;

    struct node** nodes = (struct node**)malloc(N * sizeof(struct node*));

    nodes[0] = create_node(data, id_vp, data[id_vp], N);
    print_info(nodes[0]);
    for (int i = 0; i < 3; i++) {
        vp_tree(nodes[i], nodes, node_count_ptr);
        printf("node count is: %d\n", *node_count_ptr);
    }
    for (int i = 0; i < 7; i++) {
        print_info(nodes[i]);
    }
    return 0;
}

// TODO: REMOVE DEFINE N AND d FROM THE INSERT_NODE.H FILE
//       MAKE BRAKE CONDITION FOR THE VP_TREE FUNCTION
//       CLEAR A BIT THE MAIN FUNCTION