// #include "median.h"
// #include <stdio.h>

// void swap(int* a, int* b)
// {
//     int t = *a;
//     *a = *b;
//     *b = t;
// }

// int partition(int arr[], int left, int right, int pIndex)
// {
//     // pick `pIndex` as a pivot from the array
//     int pivot = arr[pIndex];

//     // Move pivot to end
//     swap(&arr[pIndex], &arr[right]);

//     // elements < pivot, will be pushed to the left of `pIndex`;
//     // elements > pivot, will be pushed to the right of `pIndex`;
//     // equal elements can go either way
//     pIndex = left;

//     // each time we find an element less than or equal to the pivot, `pIndex`
//     // is incremented, and that element would be placed before the pivot.
//     for (int i = left; i < right; i++) {
//         if (arr[i] <= pivot) {
//             swap(&arr[i], &arr[pIndex]);
//             pIndex++;
//         }
//     }

//     // move pivot to its final place
//     swap(&arr[pIndex], &arr[right]);

//     // return `pIndex` (index of the pivot element)
//     return pIndex;
// }

// // Returns the k'th smallest element in the list within `left…right`
// int quickselect(int arr[], int left, int right, int k)
// {
//     // If the array contains only one element, return that element
//     if (left == right) {
//         // printf("Inside function calling 1st bracket value: %.2lf\n", arr[left]);

//         return arr[left];
//     }

//     // select `pIndex` between left and right
//     int pIndex = left + rand() % (right - left + 1);

//     pIndex = partition(arr, left, right, pIndex);

//     // The pivot is in its final sorted position
//     if (k == pIndex) {
//         // printf("Inside function calling 2nd bracket value: %.2lf\n", arr[k]);

//         return arr[k];

//     }

//     // if `k` is less than the pivot index
//     else if (k < pIndex) {
//         return quickselect(arr, left, pIndex - 1, k);
//     }

//     // if `k` is more than the pivot index
//     else {
//         return quickselect(arr, pIndex + 1, right, k);
//     }
// }

// // function to calculate the median of the array
// float median(int array[], int n)
// {
//     float median = 0;

//     median = (quickselect(array, 0, (rec_num_procs * proc_data_length) - 1, ((rec_num_procs * proc_data_length) / 2) - 1)
//                  + quickselect(array, 0, (rec_num_procs * proc_data_length) - 1, ((rec_num_procs * proc_data_length) / 2)))
//         / 2;
//     quickselect(array, n);
//     // if number of elements are even
//     if (n % 2 == 0)
//         median
//             = (array[(n - 1) / 2] + array[n / 2]) / 2.0;
//     // if number of elements are odd
//     else
//         median = array[n / 2];

//     return median;
// }

#include "median.h"
#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int* A, int left, int right)
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

double quickselect(int* A, int left, int right, int k)
{

    int p = partition(A, left, right);

    //k equals pivot got lucky
    if (p == k - 1) {
        return A[p];
    }
    //k less than pivot
    else if (k - 1 < p) {
        return quickselect(A, left, p - 1, k);
    }
    //k greater than pivot
    else {
        return quickselect(A, p + 1, right, k);
    }
}

double median(int* A, int n, int k)
{
    int left = 0;
    int right = n - 1;
    quickselect(A, left, right, k);

    printf("k= %d\n", k - 1);
    if ((n % 2) == 0) {
        return (((double)A[k - 2] + (double)A[k - 1]) / 2);
    }
    return A[k - 1];
}