#ifndef _ASTEROID_H_INCLUDED_
#define _ASTEROID_H_INCLUDED_

#include <SDL.h>

#define MIN_RADIUS 0.65

struct Asteroid {
    float radius;
    int edges;
    SDL_Point *points;
    SDL_Point position;
};

typedef struct Asteroid Asteroid;

/**
 * Create an asteroid.
 */
Asteroid *new_asteroid(float radius, int edges, SDL_Point position);

/**
 * Deallocate asteroid.
 */
void delete_asteroid(Asteroid *asteroid);

/**
 * Move asteroid.
 */
void move_asteroid(Asteroid *asteroid, int x_step, int y_step);

/**
 * Draw the asteroid.
 */
void draw_asteroid(Asteroid *asteroid, SDL_Renderer *renderer);

#endif
