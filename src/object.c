#include <math.h>
#include "game.h"
#include "object.h"

void wrap(float *pos, int left, int right);
void set_velocity_components(Point *velocity, float angle, float magnitude);

void set_object_props(Object *obj, int x, int y, float angle, float velocity)
{
    obj->position.x = x;
    obj->position.y = y;
    obj->angle = angle; // important! set angle before velocity
    set_object_velocity(obj, velocity);
}

void move_object(Object *obj)
{
    // update positions
    obj->position.x += obj->velocity.x * time_step;
    obj->position.y += obj->velocity.y * time_step;

    // wrap object
    wrap(&(obj->position.x), 0, game_viewport.w);
    wrap(&(obj->position.y), 0, game_viewport.h);
}

void set_object_velocity(Object *obj, float magnitude)
{
    set_velocity_components(&obj->velocity, obj->angle, magnitude);
}

void add_object_velocity(Object *obj, float magnitude)
{
    Point velocity;
    set_velocity_components(&velocity, obj->angle, magnitude);

    obj->velocity.x += velocity.x;
    obj->velocity.y += velocity.y;
}

/**
 * Checks if a value is off screen, if so, moves it to the opposite edge.
 */
void wrap(float *pos, int left, int right)
{
    if (*pos > right) {
        *pos = left;
    } else if (*pos < left) {
        *pos = right;
    }
}

/**
 * Given an angle and a magnitude, sets the velocity x and y components.
 */
void set_velocity_components(Point *velocity, float angle, float magnitude)
{
    velocity->x = cos(angle) * magnitude;
    velocity->y = sin(angle) * magnitude;
}
