#ifndef INSERT_NODE
#define INSERT_NODE

#include <stdio.h>
#include <stdlib.h>

#define N 10
#define d 2

struct node {
    double* vp;
    int id;
    double median_distance;
    double** data;
    int data_size;
    struct node* inner;
    struct node* outer;
};
struct node* create_node(double** data, int id_vp, double* vp, int size);

void vp_tree(struct node* root, struct node** nodes, int* node_counter);

void print_info(struct node* node);

#endif
