#include "insert_node.h"
#include <stdio.h>
#include <stdlib.h>

#define N 10
#define d 2

// struct node {
//     double* vp;
//     int id;
//     double median_distance;
//     double** data;
//     struct node* inner;
//     struct node* outer;
// };
struct node* create_node(double** data, int id_vp, double* vp, int size)
{
    struct node* new_node = (struct node*)malloc(sizeof(struct node));

    new_node->vp = (double*)malloc(d * sizeof(double));
    new_node->data = (double**)malloc(size * sizeof(double*));
    for (int i = 0; i < size; i++) {
        new_node->data[i] = (double*)malloc(d * sizeof(double));
    }

    new_node->data = data;

    new_node->id = id_vp;

    new_node->vp = vp;

    new_node->inner = NULL;
    new_node->outer = NULL;
    printf("node created\n");
    return new_node;
}
