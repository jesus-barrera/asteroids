#ifndef _SPACESHIP_H_INCLUDED_
#define _SPACESHIP_H_INCLUDED_

#include "polygon.h"

#define SPACESHIP_HEIGHT 25
#define SPACESHIP_BASE 13


enum {
    SHIP_TOP_VERTEX,
    SHIP_BOTTOM_LEFT_VERTEX,
    SHIP_BOTTOM_RIGHT_VERTEX,
    SHIP_NUM_VERTICES };

typedef struct Polygon Spaceship;

Spaceship *new_spaceship(int x, int y, float angle, float velocity);
void delete_spaceship(Spaceship *ship);
void move_spaceship(Spaceship *ship);
void rotate_spaceship(Spaceship *ship, float angle);
void draw_spaceship(Spaceship *ship, SDL_Renderer *renderer);

#endif
