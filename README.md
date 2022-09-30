# Parallel_Distributed_Systems_4
 Parallel Data Distribution using Vantage Point Tree

Compile

$HOME/opt/usr/local/bin/mpicc -Wall -g main.c ../inc/seq_functions/linked_list/insert_node.c ../inc/seq_functions/median.c ../inc/helpers/eucDist.c ../inc/helpers/tree_info_calc.c ../inc/threads_functions/linked_list/insert_node_threads.c ../inc/threads_functions/linked_list/distances_threads.c ../inc/threads_functions/linked_list/inner_outer_threads.c ../inc/helpers/timer.c ../inc/knn/knn_search.c ../inc/knn/knn_search_mpi.c -lpthread -o main

Please, change the codes (method_code & knn_enabled) in main.c before compiling and choose the appropiate run command.

Regular Run:

./main

MPI Run:

$HOME/opt/usr/local/bin/mpirun -np 4 ./main

// method codes: 1 = Sequential, 2 = Threads, 3 = Threshold Threads.

// knn_enabled codes: 0 = No KNN, 1 = KNN, 2 = KNN with MPI.
