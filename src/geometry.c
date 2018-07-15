#include <stdlib.h>
#include <math.h>
#include "geometry.h"
#include "vector.h"

/**
 * Checks if two line segments intersect.
 *
 * When two line segments intersect, these form the diagonals of a convex quadrilateral. The sum of
 * the interior angles of a quadrilateral is always 360 degrees (2pi radians).
 */
int lines_intersect(Point p1, Point p2, Point q1, Point q2)
{
    // direction vectors
    double dir_vector_prev[2];
    double dir_vector_next[2];

    // interior angles sum
    double sum;

    // vertex indices
    int curr, next, prev;

    // quadrilateral vertices
    Point vertices[4];

    vertices[0] = p1;
    vertices[1] = q1;
    vertices[2] = p2;
    vertices[3] = q2;

    // sum quadrilateral interior angles
    for (curr = 0, sum = 0; curr < 4; curr++) {
        next = (curr < 3) ? curr + 1 : 0;
        prev = (curr > 0) ? curr - 1 : 3;

        // get the direction vectors to the adjacent vertices (prev and next); the angle between
        // these vectors corresponds to the interior angle at the current vertex.
        get_direction_vector(dir_vector_prev, &vertices[curr], &vertices[prev]);
        get_direction_vector(dir_vector_next, &vertices[curr], &vertices[next]);

        sum += vectors_angle(dir_vector_prev, dir_vector_next, 2);
    }

    return fabs((2 * PI) - sum) < 1e-8;
}

/**
 * Get the direction vector that describes a line.
 */
double *get_direction_vector(double vector[], Point *A, Point *B)
{
    vector[0] = B->x - A->x;
    vector[1] = B->y - A->y;

    vector_unit(vector, 2);

    return vector;
}
