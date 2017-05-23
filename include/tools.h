#ifndef _TOOLS_H_INCLUDED_
#define _TOOLS_H_INCLUDED_

#include <SDL.h>

#define PI 3.14159265358979323846

typedef struct Point Point;

struct Point {
    float x;
    float y;
};

float uniform(float min, float max);
void draw_polygon(SDL_Renderer *renderer, Point *points, int points_count);

#endif
