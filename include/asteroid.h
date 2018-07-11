/**
 * Header file for asteroid's functions such create, delete, move and draw.
 *
 * Asteroids are essentially irregular polygons contained in a circle (but not
 * exactly inscribed), thus they have an associated radius which represents the
 * maximum distance between the asteroid's center and any of its vertices.
 */

#ifndef _ASTEROID_H_INCLUDED_
#define _ASTEROID_H_INCLUDED_

#include "object.h"

#define asteroid_move(asteroid) object_move((Object *)asteroid)
#define asteroid_draw(asteroid) object_draw((Object *)asteroid)
#define asteroid_delete(asteroid) object_delete((Object *)asteroid)

typedef struct Object Asteroid;

/**
 * Create a new asteroid.
 *
 * The vertices are placed randomly around the center (x and y postions) in such
 * a way they form an irregular polygon.
 */
Asteroid *asteroid_new(int x, int y, float radius, float direction, float speed, int num_points);

#endif
