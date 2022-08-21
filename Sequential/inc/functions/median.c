#include "median.h"
#include <stdio.h>
#include <stdlib.h>

void swap(double* a, double* b)
{
    double temp = *a;
    *a = *b;
    *b = temp;
}

int partition(double* A, int left, int right)
{
    int pivot = A[right];
    int i = left, x;
    for (x = left; x < right; x++) {
        if (A[x] < pivot) {
            swap(&A[i], &A[x]);
            i++;
        }
    }

    swap(&A[i], &A[right]);
    return i;
}

double quickselect(double* A, int left, int right, int k)
{

    int p = partition(A, left, right);

    // k equals pivot got lucky
    if (p == k - 1) {
        return A[p];
    }
    // k less than pivot
    else if (k - 1 < p) {
        return quickselect(A, left, p - 1, k);
    }
    // k greater than pivot
    else {
        return quickselect(A, p + 1, right, k);
    }
}

double median(double* A, int n, int k)
{
    int left = 0;
    int right = n - 1;
    quickselect(A, left, right, k);

    // printf("k= %d\n", k - 1);
    if ((n % 2) == 0) {
        return (((double)A[k - 2] + (double)A[k - 1]) / 2);
    }
    return A[k - 1];
}