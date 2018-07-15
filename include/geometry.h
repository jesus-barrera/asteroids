#ifndef _GEOMETRY_H_INCLUDED_
#define _GEOMETRY_H_INCLUDED_

#include "tools.h"

int lines_intersect(Point p1, Point p2, Point q1, Point q2);
double *get_direction_vector(double vector[], Point *A, Point *B);

#endif
