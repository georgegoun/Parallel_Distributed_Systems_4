#ifndef DISTRIBUTION
#define DISTRIBUTION

#include "../seq_functions/linked_list/insert_node.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

void distribution(int chief_rank, double** data, MPI_Comm mpiCommunicator, int data_comm_length, struct node** nodes, struct node* root, int* node_count_ptr, int d, int num_threads);
#endif