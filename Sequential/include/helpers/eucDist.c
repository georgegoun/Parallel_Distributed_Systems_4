#include "eucDist.h"
#include <math.h>
#include <stdio.h>

int eucDist(int* center, int* point, int dimensions)
{

    int distance = 0;
    for (int i = 0; i < dimensions; i++) {
        distance += pow(point[i] - center[i], 2);
    }
    // no need to get sqrt
    // distance = sqrt(distance);
    return distance;
}
