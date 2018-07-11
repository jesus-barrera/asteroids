#ifndef _OBJECT_H_INCLUDED_
#define _OBJECT_H_INCLUDED_

#include "tools.h"

/**
 * Defines a simple game's object.
 */
typedef struct Object {
    Point position;
    float radius;
    Point velocity;
    float direction;
    Point *points;
    int num_points;
} Object;

Object *object_new(int x, int y, float radius, float direction, float speed, int num_points);
void object_delete(Object *obj);
void object_move(Object *obj);
void object_update_position(Object *obj);
void object_draw(Object *obj);
void object_set_point(Object *obj, int point, float angle);
void set_velocity(Point *velocity, float direction, float speed);
SDL_bool object_is_off_screen(Object *obj);

#endif
