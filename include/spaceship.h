#ifndef _SPACESHIP_H_INCLUDED_
#define _SPACESHIP_H_INCLUDED_

#include "tools.h"

#define SPACESHIP_HEIGHT 25
#define SPACESHIP_WIDTH 13

typedef struct Spaceship Spaceship;

struct Spaceship {
    float x_velocity;
    float y_velocity;
    float angle;
    Point position;
};

Spaceship *new_spaceship(int x, int y);
void delete_spaceship(Spaceship *ship);
void move_spaceship(Spaceship *ship);
void draw_spaceship(Spaceship *ship, SDL_Renderer *renderer);

#endif
