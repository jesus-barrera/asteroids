#ifndef _SPACESHIP_H_INCLUDED_
#define _SPACESHIP_H_INCLUDED_

#include "object.h"

#define SPACESHIP_RADIUS 10

#define spaceship_update(spaceship) object_update((Object *)spaceship)
#define spaceship_delete(spaceship) object_delete((Object *)spaceship)

#define SPACESHIP_POINTS_COUNT 5
#define SPACESHIP_FLAME_POINTS_COUNT 3
#define SPACESHIP_TOTAL_POINTS_COUNT SPACESHIP_POINTS_COUNT + SPACESHIP_FLAME_POINTS_COUNT

#define SPACESHIP_TOP_VERTEX 0

typedef struct Object Spaceship;

Spaceship *spaceship_new(int x, int y, float direction, float speed);
void spaceship_draw(Spaceship *ship, SDL_bool draw_flame);

#endif
