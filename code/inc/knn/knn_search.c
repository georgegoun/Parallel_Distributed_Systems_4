#include "knn_search.h"
#include "../seq_functions/linked_list/insert_node.h"
#include "../seq_functions/median.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void knn_search(struct node** nodes, int dim, double** knn, int k, int knn_pos)
{
    struct node* current_node = nodes[0];
    double** knn = (double**)malloc(k * sizeof(double*));
    for (int i = 0; i < dim; i++) {
        knn[i] = (double*)malloc(dim * sizeof(double));
    }
    double distance = 0;

    // find the leaf node

    while (current_node->inner != NULL) {
        if (k + 1 > current_node->data_size) {
            current_node = current_node->prev;
            break;
        }
        distance = eucDist(current_node->vp, nodes[0]->data[knn_pos], dim);
        if (distance < current_node->median_distance) {
            current_node = current_node->inner;
        } else {
            current_node = current_node->outer;
        }
    }
    // current_node->data[i] ... i=idx

    // find the k nearest neighbors
    // for (int i = 0; i < current_node->data_size; i++) {
    // }
}
