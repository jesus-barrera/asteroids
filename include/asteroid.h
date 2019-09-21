/**
 * Header file for asteroid's functions such create, delete, move and draw.
 *
 * Asteroids are essentially irregular polygons contained within a circle (but not
 * exactly inscribed), thus they have an associated radius which represents the
 * maximum distance between the asteroid's center and any of its vertices.
 */

#ifndef _ASTEROID_H_INCLUDED_
#define _ASTEROID_H_INCLUDED_

#define ASTEROID_LARGE 40
#define ASTEROID_MEDIUM 20
#define ASTEROID_SMALL 10
#define ASTEROID_FRAGMENTS 2
#define ASTEROID_SPEED 60
#define ASTEROID_MIN_SIDES 10
#define ASTEROID_MAX_SIDES 15

/**
 * Fraction of the asteroid's radius to use as the minimun distance between
 * the asteroid's center and any of its vertices.
 */
#define ASTEROID_MIN_RADIUS 0.65

#include "object.h"
#include "list.h"

#define asteroid_update(asteroid) object_update((Object *)asteroid)
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

/**
 * Create multiple asteroids with random properties.
 */
Node *asteroid_create_many(int count);

#endif
