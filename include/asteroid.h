#ifndef _ASTEROID_H_INCLUDED_
#define _ASTEROID_H_INCLUDED_

#include "object.h"

#define MIN_RADIUS 0.65

typedef struct Asteroid Asteroid;

struct Asteroid {
    Object obj;
    float radius;
    int edges;
    Point *vertices;
};

Asteroid *new_asteroid(float radius, int edges, int x, int y, float angle, float velocity);
void delete_asteroid(Asteroid *asteroid);
void move_asteroid(Asteroid *asteroid);
void draw_asteroid(Asteroid *asteroid, SDL_Renderer *renderer);

#endif
