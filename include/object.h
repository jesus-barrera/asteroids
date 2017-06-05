#ifndef _OBJECT_H_INCLUDED_
#define _OBJECT_H_INCLUDED_

#include "tools.h"

typedef struct Object Object;

struct Object {
    Point velocity;
    Point position;
    float angle;
};

void set_object_props(Object *obj, int x, int y, float angle, float velocity);
void update_object_position(Object *obj);
void move_object(Object *obj);
void set_object_velocity(Object *obj, float magnitude);
void add_object_velocity(Object *obj, float magnitude);
SDL_bool is_off_screen(Object *obj);
void set_velocity_components(Point *velocity, float angle, float magnitude);

#endif
