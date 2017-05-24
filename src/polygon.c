#include "polygon.h"

void move_polygon(Polygon *polygon)
{
    int i;
    Point offset;
    Point position;

    // save position
    position = polygon->obj.position;

    move_object((Object *)polygon);

    // calculate offset
    offset.x = polygon->obj.position.x - position.x;
    offset.y = polygon->obj.position.y - position.y;

    // update vertices
    for (i = 0; i < polygon->edges; i++) {
        polygon->vertices[i].x += offset.x;
        polygon->vertices[i].y += offset.y;
    }
}

void draw_polygon(Polygon *polygon, SDL_Renderer *renderer)
{
    int from, next, last;
    Point *vertices;

    vertices = polygon->vertices;

    // draw lines
    for (from = 0, last = polygon->edges - 1; from < last; from++) {
        next = from + 1;

        SDL_RenderDrawLine(
            renderer,
            vertices[from].x, vertices[from].y,
            vertices[next].x, vertices[next].y);
    }

    // close
    SDL_RenderDrawLine(
        renderer,
        vertices[last].x, vertices[last].y,
        vertices[0].x, vertices[0].y);
}
