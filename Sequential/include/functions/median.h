#ifndef MEDIAN
#define MEDIAN

#include <stdio.h>

void swap(int* a, int* b);
int partition(int* A, int left, int right);
double quickselect(int* A, int left, int right, int k);

double median(int* A, int n, int k);

#endif