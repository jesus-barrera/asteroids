#ifndef _SPACESHIP_H_INCLUDED_
#define _SPACESHIP_H_INCLUDED_

#include "object.h"

#define SPACESHIP_RADIUS 15
#define SPACESHIP_INTERIOR_ANGLE 2.5 // between pi/2 and pi

#define spaceship_update(spaceship) object_update((Object *)spaceship)
#define spaceship_draw(spaceship) object_draw((Object *)spaceship)
#define spaceship_delete(spaceship) object_delete((Object *)spaceship)

enum {
    SPACESHIP_TOP_VERTEX,
    SPACESHIP_BOTTOM_LEFT_VERTEX,
    SPACESHIP_BOTTOM_RIGHT_VERTEX,
    SPACESHIP_NUM_VERTICES };

typedef struct Object Spaceship;

Spaceship *spaceship_new(int x, int y, float direction, float speed);

#endif
