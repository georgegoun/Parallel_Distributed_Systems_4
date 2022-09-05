#include "insert_node_threads.h"
#include "../../helpers/eucDist.h"
#include "../../seq_functions/median.h"

#include "../../seq_functions/linked_list/insert_node.h"

#include "distances_threads.h"

#include "inner_outer_threads.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// struct node {
//     double* vp;
//     int id;
//     double median_distance;
//     double** data;
//     int data_size;
//     struct node* inner;
//     struct node* outer;
// };

typedef struct Struct {
    int id;
    int num_threads;
    struct node* root;
    int dim;
    double* distances;
} makeStruct;

typedef struct Struct2 {
    int id;
    int num_threads;
    struct node* root;
    int dim;
    double* distances;
    int* data_parts_size_inner;
    int* data_parts_size_outer;
    double** data_inner;
    double** data_outer;
    int* inner;
    int* outer;
    int* lock_counter;
    int* lock_counter2;
    pthread_mutex_t* mutex;
    pthread_mutex_t* lock;
    pthread_mutex_t* lock2;
    pthread_cond_t* cond1;
    pthread_cond_t* cond2;
} makeStruct2;

void vp_tree_threads(struct node* root, struct node** nodes, int* node_counter, int d, int num_threads)
{
    // terminate condition

    if (root->data_size <= 1) {
        return;
    }

    makeStruct* arguments;
    pthread_t* threads;
    arguments = (makeStruct*)malloc(sizeof(makeStruct) * num_threads);

    threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);

    // finding the median distance

    double* distances = malloc(root->data_size * sizeof(double));

    for (int i = 0; i < num_threads; i++) {
        arguments[i].id = i;
        arguments[i].num_threads = num_threads;
        arguments[i].root = root;
        arguments[i].dim = d;
        arguments[i].distances = distances;
        pthread_create(&threads[i], NULL, distances_threads, (void*)&arguments[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    root->median_distance = median(distances, root->data_size, (root->data_size / 2) + 1);

    int* data_parts_size_inner = malloc(sizeof(int));
    int* data_parts_size_outer = malloc(sizeof(int));

    *data_parts_size_inner = 0;
    *data_parts_size_outer = root->data_size;

    makeStruct2* arguments2;
    pthread_t* threads2;

    arguments2 = (makeStruct2*)malloc(sizeof(makeStruct2) * num_threads);
    threads2 = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);

    double** data_inner = NULL;
    double** data_outer = NULL;

    int* inner = malloc(sizeof(int));
    int* outer = malloc(sizeof(int));

    *inner = 0;
    *outer = 0;

    int* lock_counter = malloc(sizeof(int));

    *lock_counter = 0;

    int* lock_counter2 = malloc(sizeof(int));

    *lock_counter2 = 0;

    pthread_mutex_t* mtx;
    mtx = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mtx, NULL);

    pthread_mutex_t* lock;
    lock = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(lock, NULL);

    pthread_mutex_t* lock2;
    lock2 = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(lock2, NULL);

    pthread_cond_t* cond1;
    cond1 = malloc(sizeof(pthread_cond_t));
    pthread_cond_init(cond1, NULL);

    pthread_cond_t* cond2;
    cond2 = malloc(sizeof(pthread_cond_t));
    pthread_cond_init(cond2, NULL);

    for (int i = 0; i < num_threads; i++) {
        arguments2[i].id = i;
        arguments2[i].num_threads = num_threads;
        arguments2[i].root = root;
        arguments2[i].dim = d;
        arguments2[i].distances = distances;
        arguments2[i].data_parts_size_inner = data_parts_size_inner;
        arguments2[i].data_parts_size_outer = data_parts_size_outer;
        arguments2[i].data_inner = data_inner;
        arguments2[i].data_outer = data_outer;
        arguments2[i].inner = inner;
        arguments2[i].outer = outer;
        arguments2[i].lock_counter = lock_counter;
        arguments2[i].lock_counter2 = lock_counter2;
        arguments2[i].mutex = mtx;
        arguments2[i].lock = lock;
        arguments2[i].lock2 = lock2;
        arguments2[i].cond1 = cond1;
        arguments2[i].cond2 = cond2;
        pthread_create(&threads2[i], NULL, inner_outer_threads, (void*)&arguments2[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads2[i], NULL);
    }
    pthread_mutex_destroy(mtx);
    pthread_mutex_destroy(lock);
    pthread_cond_destroy(cond1);

    // create inner and outer nodes
    nodes[*node_counter] = create_node(data_inner, 0, data_inner[0], *data_parts_size_inner, d);
    root->inner = nodes[*node_counter];
    (*node_counter)++;
    nodes[*node_counter] = create_node(data_outer, 0, data_outer[0], *data_parts_size_outer, d);
    root->outer = nodes[*node_counter];
    (*node_counter)++;

    printf("vp tree created\n");
}
