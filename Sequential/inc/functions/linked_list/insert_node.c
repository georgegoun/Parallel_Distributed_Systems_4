#include "insert_node.h"
#include "../../helpers/eucDist.h"
#include "../median.h"
#include <stdio.h>
#include <stdlib.h>

#define N 10
#define d 2

// struct node {
//     double* vp;
//     int id;
//     double median_distance;
//     double** data;
//     int data_size;
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

    new_node->data_size = size;

    new_node->inner = NULL;
    new_node->outer = NULL;
    printf("node created\n");
    return new_node;
}

void vp_tree(struct node* root)
{
    double* distances = malloc(root->data_size * sizeof(double));
    for (int i = 0; i < root->data_size; i++) {
        distances[i] = eucDist(root->vp, root->data[i], d);
    }

    double median_node = median(distances, root->data_size, (root->data_size / 2) + 1);

    // calculate inner and outer sizes

    int data_parts_size_inner = 0;

    int data_parts_size_outer = 0;

    for (int i = 0; i < root->data_size; i++) {
        if (distances[i] <= median_node) {
            data_parts_size_inner++;
        }
    }

    data_parts_size_outer = root->data_size - data_parts_size_inner;

    // assign each data point to inner or outer node

    double** data_inner = (double**)malloc(data_parts_size_inner * sizeof(double*));
    for (int i = 0; i < data_parts_size_inner; i++) {
        data_inner[i] = (double*)malloc(d * sizeof(double));
    }

    double** data_outer = (double**)malloc(data_parts_size_outer * sizeof(double*));
    for (int i = 0; i < data_parts_size_outer; i++) {
        data_outer[i] = (double*)malloc(d * sizeof(double));
    }

    for (int i = 0; i < root->data_size; i++) {
        int inner = 0, outer = 0;
        if (distances[i] <= median_node) {
            for (int j = 0; j < d; j++) {
                data_inner[inner][j] = root->data[i][j];
                inner++;
            }
        } else {
            for (int j = 0; j < d; j++) {
                data_outer[outer][j] = root->data[i][j];
                outer++;
            }
        }
    }

    // create inner and outer nodes
    struct node* inner_node = create_node(data_inner, 0, data_inner[0], data_parts_size_inner);
    struct node* outer_node = create_node(data_outer, 0, data_outer[0], data_parts_size_outer);
    root->inner = inner_node;
    root->outer = outer_node;
    printf("vp tree created\n");
}
