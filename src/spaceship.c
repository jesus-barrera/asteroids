#include <stdlib.h>
#include <math.h>
#include "spaceship.h"
#include "game.h"

/*
 * Spaceship points coordinates given as the distance and angle from its center.
 */
static float points[][SPACESHIP_TOTAL_POINTS_COUNT] = {
    // Spaceship's points
    {1.00, 0.00},
    {1.00, 2.50},
    {0.60, 2.65},
    {0.60, -2.65},
    {1.00, -2.50},

    // Flame's points
    {0.80, 2.85},
    {1.00, -PI},
    {0.80, -2.85},
};

static int flame_lines[] = {
    2, 5,
    5, 6,
    6, 7,
    7, 3,
    -1
};

static void spaceship_set_points(Spaceship *ship);

Spaceship *spaceship_new(int x, int y, float direction, float speed)
{
    Spaceship *ship = (Spaceship *)object_new(x, y,
                                              SPACESHIP_RADIUS, direction,
                                              speed, SPACESHIP_TOTAL_POINTS_COUNT);

    ship->outline_end = SPACESHIP_POINTS_COUNT;

    spaceship_set_points(ship);

    return ship;
}

void spaceship_draw(Spaceship *ship, SDL_bool draw_flame)
{
    object_draw((Object *)ship);

    if (draw_flame) {
        object_draw_lines((Object *)ship, flame_lines);
    }
}

void spaceship_set_points(Spaceship *ship)
{
    int i, j;

    for (i = 0, j = 8; i < j; i++) {
        // Convert to Cartesian coordinates.
        vector_set_components(
            points[i][1] + ship->direction,
            points[i][0] * ship->radius,
            &ship->points[i]);

        // Apply displacement.
        vector_add(&ship->points[i], &ship->position, &ship->points[i]);
    }
}
