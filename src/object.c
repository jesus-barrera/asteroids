#include <math.h>
#include "game.h"
#include "object.h"

void wrap(float *pos, int min, int max);

void set_object_props(Object *obj, int x, int y, float angle, float velocity)
{
    obj->position.x = x;
    obj->position.y = y;
    obj->angle = angle; // important! set angle before velocity
    set_object_velocity(obj, velocity);
}

void update_object_position(Object *obj)
{
    obj->position.x += obj->velocity.x * time_step;
    obj->position.y += obj->velocity.y * time_step;
}

void move_object(Object *obj)
{
    update_object_position(obj);

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

SDL_bool is_off_screen(Object *obj)
{
    return (obj->position.x < 0 || obj->position.x > game_viewport.w ||
            obj->position.y < 0 || obj->position.y > game_viewport.h);
}

/**
 * Checks if a value is off screen, if so, moves it to the opposite edge.
 */
void wrap(float *pos, int min, int max)
{
    if (*pos > max) {
        *pos = min;
    } else if (*pos < min) {
        *pos = max;
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
