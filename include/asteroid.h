/**
 * Header file for asteroid's functions such create, delete, move and draw.
 *
 * Asteroids are essentially irregular polygons contained in a circle (but not
 * exactly inscribed), thus they have an associated radius which represents the
 * maximum distance between the asteroid's center and any of its vertices.
 */

#ifndef _ASTEROID_H_INCLUDED_
#define _ASTEROID_H_INCLUDED_

#include "polygon.h"

typedef struct Asteroid Asteroid;

struct Asteroid {
    Polygon polygon;
    float radius;
};

/**
 * Create a new asteroid.
 *
 * The vertices are placed randomly around the center (x and y postions) in such
 * a way they form an irregular polygon.
 *
 * radius     The radius of the circle the asteroid is contained into.
 * edges      Number of edges of the asteroid.
 * x          Initial x position of the asteroid.
 * y          Initial y position of the asteroid.
 * angle      The direction of the asteroid in radians.
 * velocity   The velocity of the asteroid in pixels per second.
 */
Asteroid *new_asteroid(float radius, int edges, int x, int y, float direction, float velocity);

/**
 * Delete the specified asteroid.
 */
void delete_asteroid(Asteroid *asteroid);

/**
 * Move an asteroid by updating its x and y postion according to its direction
 * and velocity.
 *
 * asteroid   The asteroid to move.
 */
void move_asteroid(Asteroid *asteroid);

/**
 * Renders an asteroid into the specified renderer.
 *
 * asteroid The asteroid to draw.
 * renderer The renderer to draw the asteroid into.
 */
void draw_asteroid(Asteroid *asteroid, SDL_Renderer *renderer);

#endif
