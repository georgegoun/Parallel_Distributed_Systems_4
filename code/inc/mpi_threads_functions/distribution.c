#include "distribution.h"
#include "../helpers/eucDist.h"
#include "../seq_functions/linked_list/insert_node.h"
#include "../seq_functions/median.h"
#include "../threads_functions/linked_list/inner_outer_threads.h"

#include <mpi.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Struct {
    int id;
    int num_threads;
    int dim;
    double** proc_data;
    double* distances;
    int data_size;
    int median_dist;
    int* data_parts_size_inner;
    int* data_parts_size_outer;
    double** data_inner;
    double** data_outer;
    int* inner;
    int* outer;
    bool* flag;
    pthread_mutex_t* mtxin;
    pthread_mutex_t* mtxout;
} makeStruct;

// 1:

void distribution(int chief_rank, double** data, MPI_Comm mpiCommunicator, int data_comm_length, struct node** nodes, struct node* root, int* node_count_ptr, int d, int num_threads)
{
    if (data_comm_length <= 1) {
        return;
    }

    int num_procs = 0;
    int my_id = 0;

    MPI_Comm_size(mpiCommunicator, &num_procs);
    MPI_Comm_rank(mpiCommunicator, &my_id);

    double** proc_data = NULL;
    int proc_data_length = 0;

    if (my_id != num_procs - 1) {
        proc_data_length = (int)(data_comm_length / num_procs);
    } else {
        proc_data_length = (int)(data_comm_length / num_procs) + (data_comm_length % num_procs);
    }
    proc_data = (double**)malloc(proc_data_length * sizeof(double*));
    for (int i = 0; i < proc_data_length; i++) {
        proc_data[i] = (double*)malloc(d * sizeof(double));
    }
    for (int i = 0; i < proc_data_length; i++) {
        for (int j = 0; j < d; j++) {
            proc_data[i][j] = data[i + my_id * (int)(data_comm_length / num_procs)][j];
        }
    }

    double* vp = (double*)malloc(d * sizeof(double));
    double median_dist = 0;

    int* data_parts_size_inner_all = (int*)malloc(sizeof(int));
    int* data_parts_size_outer_all = (int*)malloc(sizeof(int));

    double* distances = NULL;
    double* distances_all = NULL;

    int* receive_counts = NULL;
    int* displacements = NULL;

    double** inner_all = NULL;
    double** outer_all = NULL;

    double* inner_all_1D = NULL;
    double* outer_all_1D = NULL;

    double* inner_1D = NULL;
    double* outer_1D = NULL;

    // Chief rank selectin vp

    if (my_id == chief_rank) {
        for (int i = 0; i < d; i++) {
            vp[i] = proc_data[0][i];
        }
    }

    // Broadcast vp to all ranks
    if (my_id == 0) {
        printf("ok1 \n");
    }
    MPI_Bcast(&vp[0], d, MPI_DOUBLE, chief_rank, mpiCommunicator);

    if (my_id != chief_rank) {
        distances = (double*)malloc(proc_data_length * sizeof(double));
        for (int i = 0; i < proc_data_length; i++) {
            distances[i] = eucDist(proc_data[i], vp, d);
        }
    }

    if (my_id == chief_rank) {
        distances = (double*)malloc(proc_data_length * sizeof(double));
        for (int i = 0; i < proc_data_length; i++) {
            distances[i] = eucDist(proc_data[i], vp, d);
        }
        distances_all = (double*)malloc(data_comm_length * sizeof(double));
        receive_counts = (int*)malloc(num_procs * sizeof(int));
        displacements = (int*)malloc(num_procs * sizeof(int));
        for (int i = 0; i < num_procs; i++) {
            receive_counts[i] = (int)(data_comm_length / num_procs);
            displacements[i] = i * (int)(data_comm_length / num_procs);
            if (i == num_procs - 1) {
                receive_counts[i] = (int)(data_comm_length / num_procs) + (data_comm_length % num_procs);
            }
        }
    }

    MPI_Barrier(mpiCommunicator);

    // printf("my_id: %d and proc_data_length: %d and data_comm_length: %d 1\n", my_id, proc_data_length, data_comm_length);
    //  Chief rank gather all distances

    MPI_Gatherv(distances, proc_data_length, MPI_DOUBLE, distances_all, receive_counts, displacements, MPI_DOUBLE, chief_rank, mpiCommunicator);

    MPI_Barrier(mpiCommunicator);

    // Chief rank select vp
    if (my_id == chief_rank) {
        median_dist = median(distances_all, data_comm_length, (data_comm_length / 2) + 1);
    }

    // Broadcast median_dist to all ranks

    MPI_Bcast(&median_dist, 1, MPI_DOUBLE, chief_rank, mpiCommunicator);

    // Seperate distances to all ranks

    int* data_parts_size_inner = malloc(sizeof(int));
    int* data_parts_size_outer = malloc(sizeof(int));

    *data_parts_size_inner = 0;
    *data_parts_size_outer = 0;

    bool* flag = malloc(sizeof(bool));
    *flag = 0;

    for (int i = 0; i < proc_data_length; i++) {
        if (distances[i] < median_dist) {
            *data_parts_size_inner = *data_parts_size_inner + 1;
        } else if ((distances[i] == median_dist) && *flag == 0) {
            *data_parts_size_inner = *data_parts_size_inner + 1;
            *flag = 1;
        } else if ((distances[i] == median_dist) && *flag == 1) {
            *flag = 0;
        }
    }

    *data_parts_size_outer = proc_data_length - *data_parts_size_inner;

    // Split the data

    double** data_inner = (double**)malloc(sizeof(double*) * (*data_parts_size_inner * 2));
    for (int i = 0; i < (*data_parts_size_inner * 2); i++) {
        data_inner[i] = (double*)malloc(sizeof(double) * d);
    }
    double** data_outer = (double**)malloc(sizeof(double*) * (*data_parts_size_outer * 2));
    for (int i = 0; i < (*data_parts_size_outer * 2); i++) {
        data_outer[i] = (double*)malloc(sizeof(double) * d);
    }

    int* inner = malloc(sizeof(int));
    int* outer = malloc(sizeof(int));

    *inner = 0;
    *outer = 0;

    *flag = 0;

    makeStruct* arguments;
    pthread_t* threads;

    arguments = (makeStruct*)malloc(sizeof(makeStruct) * num_threads);
    threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);

    pthread_mutex_t* mtxin;
    mtxin = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mtxin, NULL);

    pthread_mutex_t* mtxout;
    mtxout = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(mtxout, NULL);
    if (my_id == 0) {
        printf("ok2 \n");
    }
    for (int i = 0; i < num_threads; i++) {
        arguments[i].id = i;
        arguments[i].num_threads = num_threads;
        arguments[i].dim = d;
        arguments[i].proc_data = proc_data;
        arguments[i].distances = distances;
        arguments[i].data_size = proc_data_length;
        arguments[i].median_dist = median_dist;
        arguments[i].data_parts_size_inner = data_parts_size_inner;
        arguments[i].data_parts_size_outer = data_parts_size_outer;
        arguments[i].data_inner = data_inner;
        arguments[i].data_outer = data_outer;
        arguments[i].inner = inner;
        arguments[i].outer = outer;
        arguments[i].flag = flag;
        arguments[i].mtxin = mtxin;
        arguments[i].mtxout = mtxout;
        pthread_create(&threads[i], NULL, inner_outer_split_mpi, (void*)&arguments[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    MPI_Barrier(mpiCommunicator);
    if (my_id == 0) {
        printf("ok3\n");
    }
    // Tree organize construction chief
    MPI_Gather(data_parts_size_inner, 1, MPI_INT, receive_counts, 1, MPI_INT, chief_rank, mpiCommunicator);

    if (my_id == chief_rank) {
        displacements[0] = 0;
        for (int i = 0; i < num_procs; i++) {
            receive_counts[i] = receive_counts[i] * d;
        }
        for (int i = 1; i < num_procs; i++) {
            displacements[i] = displacements[i - 1] + receive_counts[i - 1];
        }

        *data_parts_size_inner_all = 0;
        for (int i = 0; i < num_procs; i++) {
            *data_parts_size_inner_all = *data_parts_size_inner_all + receive_counts[i];
        }
    }

    MPI_Bcast(data_parts_size_inner_all, 1, MPI_INT, chief_rank, mpiCommunicator);
    inner_all_1D = (double*)malloc(sizeof(double) * (*data_parts_size_inner_all));

    // make inner to 1D
    inner_1D = (double*)malloc(sizeof(double) * (*data_parts_size_inner) * d);
    int k = 0;
    for (int i = 0; i < *data_parts_size_inner; i++) {
        for (int j = 0; j < d; j++) {
            inner_1D[k] = data_inner[i][j];
            k++;
        }
    }

    // gather inner

    MPI_Gatherv(inner_1D, (*data_parts_size_inner) * d, MPI_DOUBLE, inner_all_1D, receive_counts, displacements, MPI_DOUBLE, chief_rank, mpiCommunicator);

    *data_parts_size_inner_all = *data_parts_size_inner_all / d;
    inner_all = (double**)malloc(sizeof(double*) * (*data_parts_size_inner_all));
    for (int i = 0; i < *data_parts_size_inner_all; i++) {
        inner_all[i] = (double*)malloc(sizeof(double) * d);
    }
    MPI_Bcast(inner_all_1D, (*data_parts_size_inner_all) * d, MPI_DOUBLE, chief_rank, mpiCommunicator);
    k = 0;
    for (int i = 0; i < *data_parts_size_inner_all; i++) {
        for (int j = 0; j < d; j++) {
            inner_all[i][j] = inner_all_1D[k];
            k++;
        }
    }
    // if (my_id == 2) {
    //     for (int i = 0; i < *data_parts_size_inner_all; i++) {
    //         for (int j = 0; j < d; j++) {
    //             printf("inner_all(%d)%f ", my_id, inner_all[i][j]);
    //         }
    //         printf("\n");
    //     }
    // }
    MPI_Barrier(mpiCommunicator);

    MPI_Gather(data_parts_size_outer, 1, MPI_INT, receive_counts, 1, MPI_INT, chief_rank, mpiCommunicator);

    if (my_id == chief_rank) {
        displacements[0] = 0;
        for (int i = 0; i < num_procs; i++) {
            receive_counts[i] = receive_counts[i] * d;
        }
        for (int i = 1; i < num_procs; i++) {
            displacements[i] = displacements[i - 1] + receive_counts[i - 1];
        }

        *data_parts_size_outer_all = 0;
        for (int i = 0; i < num_procs; i++) {
            *data_parts_size_outer_all = *data_parts_size_outer_all + receive_counts[i];
        }
    }
    MPI_Bcast(data_parts_size_outer_all, 1, MPI_INT, chief_rank, mpiCommunicator);
    outer_all_1D = (double*)malloc(sizeof(double) * (*data_parts_size_outer_all));

    // make outer to 1D
    outer_1D = (double*)malloc(sizeof(double) * (*data_parts_size_outer) * d);
    k = 0;
    for (int i = 0; i < *data_parts_size_outer; i++) {
        for (int j = 0; j < d; j++) {
            outer_1D[k] = data_outer[i][j];
            k++;
        }
    }

    // gather outer

    MPI_Gatherv(outer_1D, (*data_parts_size_outer) * d, MPI_DOUBLE, outer_all_1D, receive_counts, displacements, MPI_DOUBLE, chief_rank, mpiCommunicator);

    *data_parts_size_outer_all = *data_parts_size_outer_all / d;
    outer_all = (double**)malloc(sizeof(double*) * (*data_parts_size_outer_all));
    for (int i = 0; i < *data_parts_size_outer_all; i++) {
        outer_all[i] = (double*)malloc(sizeof(double) * d);
    }
    MPI_Bcast(outer_all_1D, (*data_parts_size_outer_all) * d, MPI_DOUBLE, chief_rank, mpiCommunicator);
    k = 0;
    for (int i = 0; i < *data_parts_size_outer_all; i++) {
        for (int j = 0; j < d; j++) {
            outer_all[i][j] = outer_all_1D[k];
            k++;
        }
    }
    // if (my_id == 3) {
    //     for (int i = 0; i < *data_parts_size_outer_all; i++) {
    //         for (int j = 0; j < d; j++) {
    //             printf("outer_all(%d)%f ", my_id, outer_all[i][j]);
    //         }
    //         printf("\n");
    //     }
    // }

    if (my_id == 0) {
        printf("ok4\n");
    }

    MPI_Barrier(mpiCommunicator);
    //  printf("demi  \t(%d)\n", my_id);
    // Chief creates the tree
    if (my_id == chief_rank) {
        // Create the tree

        nodes[*node_count_ptr] = create_node(inner_all, 0, inner_all[0], *data_parts_size_inner_all, d);

        printf("size %d\n", nodes[*node_count_ptr]->data_size);
        // printf("root size %d\n", root->data_size);
        printf("wow: %d\n", *node_count_ptr);

        if (root->inner == NULL) {
            printf("(%d)b:root is null\n", *node_count_ptr);
        } else {
            printf("(%d)b:root is not null\n", *node_count_ptr);
        }
        root->inner = nodes[*node_count_ptr];
        if (root->inner == NULL) {
            printf("(%d)a:root is null\n", *node_count_ptr);
        } else {
            printf("(%d):root is not null\n", *node_count_ptr);
        }
        // if (root->inner == NULL) {
        //     printf("NULL\n");
        // }

        nodes[*node_count_ptr]->prev = root;
        printf("wow\n");
        *node_count_ptr = *node_count_ptr + 1;
        // printf("root size %d\n", root->data_size);
    }
    MPI_Bcast(node_count_ptr, 1, MPI_INT, chief_rank, mpiCommunicator);
    if (my_id != chief_rank) {
        nodes[*node_count_ptr - 1] = create_node(inner_all, 0, inner_all[0], *data_parts_size_inner_all, d);
    }
    printf("finish\t(%d)\n", my_id);
    MPI_Barrier(mpiCommunicator);
    distribution(0, inner_all, mpiCommunicator, *data_parts_size_inner_all, nodes, nodes[*node_count_ptr - 1], node_count_ptr, d, num_threads);

    if (my_id == chief_rank) {
        nodes[*node_count_ptr] = create_node(outer_all, 0, outer_all[0], *data_parts_size_outer_all, d);
        root->outer = nodes[*node_count_ptr];
        nodes[*node_count_ptr]->prev = root;
        *node_count_ptr = *node_count_ptr + 1;
        printf("ok2\t\n");
    } else {
        *node_count_ptr = 0;
    }
    distribution(0, outer_all, mpiCommunicator, *data_parts_size_outer_all, nodes, nodes[*node_count_ptr - 1], node_count_ptr, d, num_threads);

    // printf("my_id: %d my median dist: %f", my_id, median_dist);
    return;
}
