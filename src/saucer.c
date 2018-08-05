#include "saucer.h"
#include "game.h"

#define OPPOSITE_VERTEX(I) SAUCER_VERTICES_COUNT - (I + 1)

/*
 * Displacement of the saucer vertices from its center in a saucer of radius one.
 * This can be used to place the vertices of a saucer of any radius. Since the
 * saucer is symmetrical, only four of the eight vertices are defined here; the
 * other four are obtained by reflecting these points over the y axis.
 */
static Point points[] = {
    {0.38, 0.4},
    {1, 0},
    {0.38, -0.3},
    {0.28, -0.6},
};

static int inside_lines[] = {
    1, 6,
    2, 5,
    -1
};

static void saucer_set_points(Saucer *saucer);

Saucer *saucer_new(int x, int y, float radius, float direction, float speed)
{
    Saucer *saucer = (Saucer *)object_new(x, y, radius, direction,
                                          speed, SAUCER_VERTICES_COUNT);

    saucer_set_points(saucer);

    return saucer;
}

void saucer_draw(Saucer *saucer)
{
    object_draw((Object *)saucer);
    object_draw_lines((Object *)saucer, inside_lines);
}

void saucer_set_points(Saucer *saucer)
{
    int i, j, k;

    for (i = 0, j = SAUCER_VERTICES_COUNT / 2; i < j; i++) {
        // Calculate vertex position from the center.
        vector_mult_by_scalar(&points[i], saucer->radius, &saucer->points[i]);

        // Index of the opposite vertex; reflect the point over the y axis.
        k = OPPOSITE_VERTEX(i);

        saucer->points[k] = saucer->points[i];
        saucer->points[k].x *= -1;

        // Apply displacement.
        vector_add(&saucer->points[i], &saucer->position, &saucer->points[i]);
        vector_add(&saucer->points[k], &saucer->position, &saucer->points[k]);
    }
}
