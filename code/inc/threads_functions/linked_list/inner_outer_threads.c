#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../seq_functions/linked_list/insert_node.h"
#include "inner_outer_threads.h"

typedef struct Struct {
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
    pthread_mutex_t* mutex;
    pthread_mutex_t* lock;
} makeStruct;

void* inner_outer_threads(void* args)
{
    makeStruct* arg = (makeStruct*)args;
    int range = arg->root->data_size / arg->num_threads;
    int start = arg->id * range;
    int end = start + range;
    if (arg->id == arg->num_threads - 1) {
        end = arg->root->data_size;
    }

    // calculate inner and outer sizes

    for (int i = start; i < end; i++) {
        if (arg->distances[i] <= arg->root->median_distance) {
            pthread_mutex_lock(arg->mutex);
            *(arg->data_parts_size_inner)++;
            *(arg->data_parts_size_outer)--;
            pthread_mutex_unlock(arg->mutex);
        }
    }
    pthread_mutex_lock(arg->mutex);
    (*lock_counter)++;
    pthread_mutex_unlock(arg->mutex);

    if (*(lock_counter) == arg->num_threads) {
        *(lock_counter) = 0;
        pthread_cond_signal(&cond1);
    } else {
        pthread_cond_wait(&cond1, &lock);
    }

    // await all threads to finish with signal

    // malloc data_inner and data_outer

    if (arg->id == 0) {
        arg->data_inner = (double**)malloc(sizeof(double*) * (*data_parts_size_inner));
        for (int i = 0; i < (*data_parts_size_inner); i++) {
            arg->data_inner[i] = (double*)malloc(sizeof(double) * arg->dim);
        }
        arg->data_outer = (double**)malloc(sizeof(double*) * (*data_parts_size_outer));
        for (int i = 0; i < (*data_parts_size_outer); i++) {
            arg->data_outer[i] = (double*)malloc(sizeof(double) * arg->dim);
        }
    }
    // wait for all threads to finish
    pthread_barrier_wait(&barrier);

    // assign each data point to inner or outer node
    for (int i = start; i < end; i++) {
        if (arg->distances[i] <= arg->root->median_distance) {
            pthread_mutex_lock(arg->mutex);
            for (int j = 0; j < arg->dim; j++) {
                arg->data_inner[*(arg->inner) - 1][j] = arg->root->data[i][j];
            }
            *(arg->inner)++;
            pthread_mutex_unlock(arg->mutex);
        } else {
            pthread_mutex_lock(arg->mutex);
            for (int j = 0; j < arg->dim; j++) {
                arg->data_outer[*(arg->outer) - 1][j] = arg->root->data[i][j];
            }
            *(arg->outer)--;
            pthread_mutex_unlock(arg->mutex);
        }
    }
    return NULL;
}