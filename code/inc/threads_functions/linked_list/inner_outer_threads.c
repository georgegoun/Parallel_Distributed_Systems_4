#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    //    double** data_inner;
    //    double** data_outer;
    //    int* inner;
    //    int* outer;
    //    int* lock_counter;
    //    int* lock_counter2;
    pthread_mutex_t* mutex;
    //    pthread_mutex_t* lock;
    //    pthread_mutex_t* lock2;
    //    pthread_cond_t* cond1;
    //    pthread_cond_t* cond2;
} makeStruct;

typedef struct Struct3 {
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
    pthread_mutex_t* mtxin;
    pthread_mutex_t* mtxout;
} makeStruct3;

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
    // printf("idsssdadas\n");
    // sleep(1);
    for (int i = start; i < end; i++) {
        if (arg->distances[i] <= arg->root->median_distance) {
            pthread_mutex_lock(arg->mutex);
            (*arg->data_parts_size_inner)++;
            (*arg->data_parts_size_outer)--;
            pthread_mutex_unlock(arg->mutex);
        }
    }
    /*
        // prepare conditions to wait for other threads

        pthread_mutex_lock(arg->lock);
        (*arg->lock_counter)++;
        // printf("BFOR: id: %d____lock counter is: %d\n", arg->id, *arg->lock_counter);
        pthread_mutex_unlock(arg->lock);

        // wait for all threads to finish

        pthread_mutex_lock(arg->lock);
        while (*arg->lock_counter < arg->num_threads) {
            // printf("WAIT: id: %d____lock counter is: %d\n", arg->id, *arg->lock_counter);
            pthread_cond_wait(arg->cond1, arg->lock);
        }
        pthread_mutex_unlock(arg->lock);

        pthread_mutex_lock(arg->lock);
        if ((*arg->lock_counter) == arg->num_threads) {
            // printf("FREE: id: %d____lock counter is: %d\n", arg->id, *arg->lock_counter);
            *arg->lock_counter = *arg->lock_counter + 1;
            pthread_cond_broadcast(arg->cond1);
        }
        pthread_mutex_unlock(arg->lock);

        // malloc data_inner and data_outer

        if (arg->id == 0) {
            arg->data_inner = (double**)malloc(sizeof(double*) * (*arg->data_parts_size_inner));
            for (int i = 0; i < (*arg->data_parts_size_inner); i++) {
                arg->data_inner[i] = (double*)malloc(sizeof(double) * arg->dim);
            }
            arg->data_outer = (double**)malloc(sizeof(double*) * (*arg->data_parts_size_outer));
            for (int i = 0; i < (*arg->data_parts_size_outer); i++) {
                arg->data_outer[i] = (double*)malloc(sizeof(double) * arg->dim);
            }
        }

        // prepare conditions to wait for other threads

        pthread_mutex_lock(arg->lock2);
        (*arg->lock_counter2)++;
        // printf("BFOR: id: %d____lock counter is: %d\n", arg->id, *arg->lock_counter);
        pthread_mutex_unlock(arg->lock2);

        // wait for all threads to finish

        pthread_mutex_lock(arg->lock2);
        while (*arg->lock_counter2 < arg->num_threads) {
            // printf("WAIT: id: %d____lock counter is: %d\n", arg->id, *arg->lock_counter2);
            pthread_cond_wait(arg->cond2, arg->lock2);
        }
        pthread_mutex_unlock(arg->lock2);

        pthread_mutex_lock(arg->lock2);
        if ((*arg->lock_counter2) == arg->num_threads) {
            // printf("FREE: id: %d____lock counter is: %d\n", arg->id, *arg->lock_counter2);
            *arg->lock_counter2 = *arg->lock_counter2 + 1;
            pthread_cond_broadcast(arg->cond2);
        }
        pthread_mutex_unlock(arg->lock2);

        printf("id: %d____ sizes: in_%d out_%d arg_in %d arg_out %d arg_dim %d i_start %d i_end %d\n", arg->id, *arg->data_parts_size_inner, *arg->data_parts_size_outer, *arg->inner, *arg->outer, arg->dim, start, end);

        // assign each data point to inner or outer node
        for (int i = start; i < end; i++) {
            if (arg->distances[i] <= arg->root->median_distance) {
                pthread_mutex_lock(arg->mutex);
                // printf("OK: id: %d____\n", arg->id);
                printf("!!OK: id: %d____\n", arg->id);
                for (int j = 0; j < arg->dim; j++) {
                    arg->data_inner[*(arg->inner)][j] = arg->root->data[i][j];
                }

                (*arg->inner) = (*arg->inner) + 1;
                pthread_mutex_unlock(arg->mutex);
            } else {
                // pthread_mutex_lock(arg->lock);
                // // printf("ELSE!: OK: id: %d____, out %d arg_out %d\n", arg->id, *arg->data_parts_size_outer, *arg->outer);
                // for (int j = 0; j < arg->dim; j++) {
                //     printf("arg_outer: %d i: %d data:%f \n", *(arg->outer), i, arg->root->data[i][j]);
                //     arg->data_outer[*(arg->outer)][j] = arg->root->data[i][j];
                //     // printf("ELSE!!: OK: id: %d____\n", arg->id);
                // }
                // printf("ELSE: OK: id: %d____\n", arg->id);

                // (*arg->outer) = (*arg->outer) + 1;
                // pthread_mutex_unlock(arg->lock);
            }
        }

        printf("OK: id: %d____\n", arg->id);
    */
    return NULL;
}

void* inner_outer_split(void* args)
{
    makeStruct3* arg = (makeStruct3*)args;
    int range = arg->root->data_size / arg->num_threads;
    int start = arg->id * range;
    int end = start + range;
    if (arg->id == arg->num_threads - 1) {
        end = arg->root->data_size;
    }
    // assign each data point to inner or outer node
    for (int i = start; i < end; i++) {
        if (arg->distances[i] <= arg->root->median_distance) {
            pthread_mutex_lock(arg->mtxin);
            for (int j = 0; j < arg->dim; j++) {
                arg->data_inner[*(arg->inner)][j] = arg->root->data[i][j];
            }
            (*arg->inner) = (*arg->inner) + 1;
            pthread_mutex_unlock(arg->mtxin);
        } else {
            pthread_mutex_lock(arg->mtxout);
            for (int j = 0; j < arg->dim; j++) {
                arg->data_outer[*(arg->outer)][j] = arg->root->data[i][j];
            }
            (*arg->outer) = (*arg->outer) + 1;
            pthread_mutex_unlock(arg->mtxout);
        }
    }
    return NULL;
}

// TODO: REPAIR LINE 111 TILL HERE