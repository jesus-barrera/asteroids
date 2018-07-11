#include <stdlib.h>
#include <math.h>
#include "spaceship.h"
#include "game.h"

void spaceship_set_vertices(Spaceship *ship);

Spaceship *spaceship_new(int x, int y, float direction, float speed)
{
    Spaceship *ship = (Spaceship *)object_new(x, y,
                                              SPACESHIP_RADIUS, direction,
                                              speed, SPACESHIP_NUM_VERTICES);

    spaceship_set_vertices(ship);

    return ship;
}

void spaceship_rotate(Spaceship *ship, float angle)
{
    ship->direction += angle;
    spaceship_set_vertices(ship);
}

void spaceship_set_vertices(Spaceship *ship)
{
    object_set_point((Object *)ship,
                     SPACESHIP_TOP_VERTEX,
                     0);

    object_set_point((Object *)ship,
                     SPACESHIP_BOTTOM_LEFT_VERTEX,
                     SPACESHIP_INTERIOR_ANGLE);

    object_set_point((Object *)ship,
                     SPACESHIP_BOTTOM_RIGHT_VERTEX,
                     SPACESHIP_INTERIOR_ANGLE * -1);
}
