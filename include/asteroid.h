#ifndef _ASTEROID_H_INCLUDED_
#define _ASTEROID_H_INCLUDED_

#include <SDL.h>
#include "tools.h"

#define MIN_RADIUS 0.65

typedef struct Asteroid Asteroid;

struct Asteroid {
    float radius;
    float velocity;
    float angle;
    int edges;
    Point position;
    Point *points;
};

/**
 * Create an asteroid.
 */
Asteroid *new_asteroid(float radius, int edges, int x, int y);

/**
 * Deallocate asteroid.
 */
void delete_asteroid(Asteroid *asteroid);

/**
 * Move asteroid.
 */
void move_asteroid(Asteroid *asteroid);

/**
 * Draw the asteroid.
 */
void draw_asteroid(Asteroid *asteroid, SDL_Renderer *renderer);

#endif
