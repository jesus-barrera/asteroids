#ifndef _POLYGON_H_INCLUDED_
#define _POLYGON_H_INCLUDED_

#include "object.h"
#include "geometry.h"

typedef struct Polygon Polygon;

struct Polygon {
    Object obj;
    Point *vertices;
    int edges;
};

void move_polygon(Polygon *polygon);
void draw_polygon(Polygon *polygon, SDL_Renderer *renderer);
SDL_bool polygon_intersect_line(Polygon *polygon, Point line[]);

#endif
