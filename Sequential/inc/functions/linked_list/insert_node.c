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
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < d; j++) {
            new_node->data[i][j] = data[i][j];
        }
    }
    // new_node->data = data;

    new_node->id = id_vp;

    new_node->vp = vp;

    new_node->data_size = size;

    new_node->inner = NULL;
    new_node->outer = NULL;
    printf("node created...\n");
    return new_node;
}

void vp_tree(struct node* root, struct node** nodes, int* node_counter)
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
    int inner = 0, outer = 0;

    for (int i = 0; i < root->data_size; i++) {
        if (distances[i] <= median_node) {
            for (int j = 0; j < d; j++) {

                data_inner[inner][j] = root->data[i][j];
            }
            inner++;

        } else {
            for (int j = 0; j < d; j++) {
                data_outer[outer][j] = root->data[i][j];
            }
            outer++;
        }
    }

    // create inner and outer nodes
    nodes[*node_counter] = create_node(data_inner, 0, data_inner[0], data_parts_size_inner);
    root->inner = nodes[*node_counter];
    (*node_counter)++;
    nodes[*node_counter] = create_node(data_outer, 0, data_outer[0], data_parts_size_outer);
    root->outer = nodes[*node_counter];
    (*node_counter)++;

    printf("vp tree created\n");
}

void print_info(struct node* node)
{
    printf("\n\n--------------------\n\n");
    printf("node id: %d\n", node->id);
    printf("node vp: ");
    for (int i = 0; i < d; i++) {
        printf("%f\t", node->vp[i]);
    }
    printf("\n");
    printf("node data size: %d\nmedian distance: %f\n\n-- -- -\tdata\t-- -- -\n",
        node->data_size,
        node->median_distance);
    for (int i = 0; i < node->data_size; i++) {
        for (int j = 0; j < d; j++) {
            printf("%f ", node->data[i][j]);
        }
        printf("\n");
    }
    if (node->inner != NULL) {
        printf("we got an inner child!\n");
        // printf("inner node:\n");
        // print_info(node->inner);
    }
    if (node->outer != NULL) {
        printf("we got an outer child!\n");
        // printf("outer node:\n");
        // print_info(node->outer);
    }
    printf("\n\n--------------------\n\n");
}