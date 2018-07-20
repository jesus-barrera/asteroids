#include <stdlib.h>
#include <math.h>
#include "game.h"
#include "asteroid.h"

static void asteroid_set_vertices(Asteroid *asteroid);

Asteroid *asteroid_new(int x, int y, float radius, float direction, float speed, int num_points)
{
    Asteroid *asteroid = (Asteroid *)object_new(x, y,
                                                radius, direction,
                                                speed, num_points);

    asteroid_set_vertices(asteroid);

    return asteroid;
}

void asteroid_set_vertices(Asteroid *asteroid)
{
    int i;
    float angle, angle_step, angle_min, angle_max, radius;

    angle_step = 2 * PI / asteroid->num_points;
    angle_min = 0;

    for (i = 0; i < asteroid->num_points; i++) {
        angle_max = angle_min + angle_step;

        radius = uniform(asteroid->radius * ASTEROID_MIN_RADIUS, asteroid->radius);
        angle = uniform(angle_min, angle_max);

        asteroid->points[i].x = asteroid->position.x + cos(angle) * radius;
        asteroid->points[i].y = asteroid->position.y + sin(angle) * radius;

        angle_min = angle_max;
    }
}
