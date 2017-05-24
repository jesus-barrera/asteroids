#ifndef _ASTEROID_H_INCLUDED_
#define _ASTEROID_H_INCLUDED_

#include "polygon.h"

#define MIN_RADIUS 0.65

typedef struct Asteroid Asteroid;

struct Asteroid {
    Polygon polygon;
    float radius;
};

Asteroid *new_asteroid(float radius, int edges, int x, int y, float angle, float velocity);
void delete_asteroid(Asteroid *asteroid);
void move_asteroid(Asteroid *asteroid);
void draw_asteroid(Asteroid *asteroid, SDL_Renderer *renderer);

#endif
