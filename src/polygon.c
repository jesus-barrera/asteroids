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

/**
 * Returns SDL_TRUE if there's an intersection between a polygon and a line; SDL_FALSE otherwise.
 *
 * An intersection exists if the line cuts any of the polygon edges.
 */
SDL_bool polygon_intersect_line(Polygon *polygon, Point line[])
{
    // vertex indices
    int curr, next, last;

    // each polygon edge is described by two points
    Point edge[2];

    // check if the line intersects any edge
    for (curr = 0, last = polygon->edges - 1; curr < polygon->edges; curr++) {
        next = (curr < last) ? curr + 1 : 0;

        edge[0] = polygon->vertices[curr];
        edge[1] = polygon->vertices[next];

        if (lines_intersect(line, edge)) {
            return SDL_TRUE;
        }
    }

    return SDL_FALSE;
}
