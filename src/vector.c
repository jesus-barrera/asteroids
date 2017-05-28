#include <math.h>
#include "vector.h"
#include <stdio.h>

double vectors_angle(double a_vector[], double b_vector[], int dimension)
{
    return acos(vectors_dot_product(a_vector, b_vector, dimension)
                / vector_norm(a_vector, dimension) * vector_norm(b_vector, dimension));
}

double vectors_dot_product(double a_vector[], double b_vector[], int dimension)
{
    double sum;
    int i;

    for (i = 0, sum = 0; i < dimension; i++) {
        sum += a_vector[i] * b_vector[i];
    }

    return sum;
}

double vector_norm(double vector[], int dimension)
{
    return sqrt( vectors_dot_product(vector, vector, dimension) );
}

/**
 * Converts a given vector into a vector of length 1 and same direction.
 */
double vector_unit(double vector[], int dimension)
{
    double magnitude;
    int i;

    magnitude = vector_norm(vector, dimension);

    for (i = 0; i < dimension; i++) {
        vector[i] /= magnitude;
    }
}
