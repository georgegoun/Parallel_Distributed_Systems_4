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
    struct node* inner;
    struct node* outer;
};
struct node* create_node(double** data, int id_vp, double* vp, int size);

#endif
