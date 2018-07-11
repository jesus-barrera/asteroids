#include <math.h>
#include <stdlib.h>
#include "game.h"
#include "object.h"

static void wrap(float *pos, int min, int max);

Object *object_new(int x, int y, float radius, float direction, float speed, int num_points)
{
    Object *obj = (Object *)malloc(sizeof(Object));

    obj->position.x = x;
    obj->position.y = y;
    obj->radius = radius;
    obj->direction = direction;
    obj->num_points = num_points;
    obj->points = (Point *)malloc(sizeof(Point) * num_points);

    set_velocity(&obj->velocity, direction, speed);

    return obj;
}

void object_delete(Object *obj)
{
    free(obj->points);
    free(obj);
}

void object_move(Object *obj)
{
    int i;
    Point offset;
    Point position;

    position = obj->position;

    object_update_position(obj);

    wrap(&(obj->position.x), 0, game_viewport.w);
    wrap(&(obj->position.y), 0, game_viewport.h);

    offset.x = obj->position.x - position.x;
    offset.y = obj->position.y - position.y;

    for (i = 0; i < obj->num_points; i++) {
        obj->points[i].x += offset.x;
        obj->points[i].y += offset.y;
    }
}

void object_update_position(Object *obj)
{
    obj->position.x += obj->velocity.x * time_step;
    obj->position.y += obj->velocity.y * time_step;
}

void object_draw(Object *obj)
{
    int from, next, last;

    for (from = 0, last = obj->num_points; from < last; from++) {
        next = (from + 1) % last;

        SDL_RenderDrawLine(
            renderer,
            obj->points[from].x, obj->points[from].y,
            obj->points[next].x, obj->points[next].y);
    }
}

void object_set_point(Object *obj, int point, float angle)
{
    angle += obj->direction;

    obj->points[point].x = obj->position.x + cos(angle) * obj->radius;
    obj->points[point].y = obj->position.y + sin(angle) * obj->radius;
}

SDL_bool object_is_off_screen(Object *obj)
{
    return (obj->position.x < 0 || obj->position.x > game_viewport.w ||
            obj->position.y < 0 || obj->position.y > game_viewport.h);
}

void set_velocity(Point *velocity, float direction, float speed)
{
    velocity->x = cos(direction) * speed;
    velocity->y = sin(direction) * speed;
}

void wrap(float *pos, int min, int max)
{
    if (*pos > max) {
        *pos = min;
    } else if (*pos < min) {
        *pos = max;
    }
}
