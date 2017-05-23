#include <stdlib.h>
#include "tools.h"

float uniform(float min, float max)
{
    return (rand() / (float)RAND_MAX) * (max - min) + min;
}

void draw_polygon(SDL_Renderer *renderer, Point *points, int points_count)
{
    int from, next, last;

    for (from = 0, last = points_count - 1; from < last; from++) {
        next = from + 1;

        SDL_RenderDrawLine(
            renderer,
            points[from].x, points[from].y,
            points[next].x, points[next].y);
    }

    // close
    SDL_RenderDrawLine(
        renderer,
        points[last].x, points[last].y,
        points[0].x, points[0].y);
}
