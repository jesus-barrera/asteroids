#ifndef _OBJECT_H_INCLUDED_
#define _OBJECT_H_INCLUDED_

#include "tools.h"
#include "list.h"

/**
 * Defines a simple game's object.
 */
typedef struct Object {
    Point position;
    int radius;
    Point velocity;
    float direction;
    float angular_speed;
    SDL_bool wrap;
    Point *points;
    int num_points;
    int outline_end;
} Object;

Object *object_new(int x, int y, int radius, float direction, float speed, int num_points);
void object_set_props(Object *obj, int x, int y, int radius, float direction, float speed, int num_points);
void object_delete(Object *obj);
void object_update(Object *obj);
void object_move(Object *obj, Point *displacement);
void object_rotate(Object *obj, float radians);
SDL_bool object_check_collision(Object *a, Object *b);
SDL_bool object_circle_collision(Object *a, Object *b);
SDL_bool object_intersect_line(Object *a, Point p1, Point p2);
void object_draw(Object *obj);
void object_draw_lines(Object *obj, int *points);
void object_set_point(Object *obj, int point, float angle);
SDL_bool object_is_off_screen(Object *obj);
void object_reset(Object *obj, int x, int y, float direction, float speed);

void objects_draw(Node *node);
void objects_update(Node *node);

void vector_set_components(float direction, float magnitude, Point *result);
void vector_add(Point *v, Point *u, Point *result);
void vector_diff(Point *v, Point *u, Point *result);
void vector_mult_by_scalar(Point *v, float scalar, Point *result);

#endif
