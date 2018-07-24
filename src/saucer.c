#include "saucer.h"
#include "game.h"

#define OPPOSITE_VERTEX(I) SAUCER_VERTICES_COUNT - (I + 1)

/*
 * Displacement of the saucer vertices from its center in a saucer of radius one.
 * This can be used to place the vertices of a saucer of any radius. Since the
 * saucer is symmetrical, only four of the eight vertices are defined here; the
 * other four are obtained by reflecting these points over the y axis.
 */
Point vertices[] = {
    {0.38, 0.4},
    {1, 0},
    {0.38, -0.3},
    {0.28, -0.6},
};

static void saucer_set_vertices(Saucer *saucer);
static void saucer_draw_join(Saucer *saucer, int i);

Saucer *saucer_new(int x, int y, float radius, float direction, float speed)
{
    Saucer *saucer = (Saucer *)object_new(x, y, radius, direction,
                                          speed, SAUCER_VERTICES_COUNT);

    saucer_set_vertices(saucer);

    return saucer;
}

void saucer_draw(Saucer *saucer)
{
    int i, j;

    object_draw((Object *)saucer);

    // Draw aditional lines; these lines join two opposite vertices.
    saucer_draw_join(saucer, 1);
    saucer_draw_join(saucer, 2);
}

void saucer_set_vertices(Saucer *saucer)
{
    int i, j, k;

    for (i = 0, j = SAUCER_VERTICES_COUNT / 2; i < j; i++) {
        // Calculate vertex position from the center.
        vector_mult_by_scalar(&vertices[i], saucer->radius, &saucer->points[i]);

        // Index of the opposite vertex; reflect the point over the y axis.
        k = OPPOSITE_VERTEX(i);

        saucer->points[k] = saucer->points[i];
        saucer->points[k].x *= -1;

        // Apply displacement.
        vector_add(&saucer->points[i], &saucer->position, &saucer->points[i]);
        vector_add(&saucer->points[k], &saucer->position, &saucer->points[k]);
    }
}

void saucer_draw_join(Saucer *saucer, int i)
{
    int j = OPPOSITE_VERTEX(i);

    SDL_RenderDrawLine(
        renderer,
        saucer->points[i].x, saucer->points[i].y,
        saucer->points[j].x, saucer->points[j].y);
}
