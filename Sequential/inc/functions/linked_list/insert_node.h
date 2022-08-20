#ifndef INSERT_NODE
#define INSERT_NODE

#include <stdio.h>
#include <stdlib.h>

#define N 100
#define d 2

struct node {
    double vantage_point[d];
    int id;
    double median_distance;
    double** data;
    struct node* inner;
    struct node* outer;
};
void create_node(double** data, int size);

#endif
