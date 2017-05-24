#ifndef _OBJECT_H_INCLUDED_
#define _OBJECT_H_INCLUDED_

#include "tools.h"

typedef struct Object Object;

struct Object {
    Point velocity;
    Point position;
    float angle;
};

void move_object(Object *obj);
void set_object_velocity(Object *obj, float magnitude);
void add_object_velocity(Object *obj, float magnitude);

#endif
