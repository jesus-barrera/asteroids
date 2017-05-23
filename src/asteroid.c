#include <math.h>
#include "asteroid.h"

#define PI 3.14159265358979323846

static void set_asteroid_points(Asteroid *asteroid);

float uniform(float min, float max)
{
    return (rand() / (float)RAND_MAX) * (max - min) + min;
}

/**
 * Create a new asteroid.
 */
Asteroid *new_asteroid(float radius, int edges, SDL_Point position)
{
    Asteroid *asteroid = (Asteroid *)malloc(sizeof(Asteroid));

    asteroid->radius = radius;
    asteroid->edges = edges;
    asteroid->position = position;

    set_asteroid_points(asteroid);

    return asteroid;
}

/**
 * Sets the asteroid draw points.
 */
void set_asteroid_points(Asteroid *asteroid)
{
    int i;
    float angle, angle_step, angle_min, angle_max, radius;

    asteroid->points = (SDL_Point *)malloc(sizeof(SDL_Point) * asteroid->edges);

    angle_step = 2 * PI / asteroid->edges;
    angle_min = 0;

    for (i = 0; i < asteroid->edges; i++) {
        angle_max = angle_min + angle_step;

        radius = uniform(asteroid->radius * MIN_RADIUS, asteroid->radius);
        angle = uniform(angle_min, angle_max);

        asteroid->points[i].x = asteroid->position.x + cos(angle) * radius;
        asteroid->points[i].y = asteroid->position.y + sin(angle) * radius;

        angle_min = angle_max;
    }
}

/**
 * Deallocate asteroid.
 */
void delete_asteroid(Asteroid *asteroid)
{
    free(asteroid->points);
    free(asteroid);
}

/**
 * Move asteroid.
 */
void move_asteroid(Asteroid *asteroid, int x_step, int y_step)
{
    int i;

    for (i = 0; i < asteroid->edges; i++) {
        asteroid->points[i].x += x_step;
        asteroid->points[i].y += y_step;
    }
}

/**
 * Draw the asteroid.
 */
void draw_asteroid(Asteroid *asteroid, SDL_Renderer *renderer)
{
    int from, next, last;

    SDL_Point *points = asteroid->points;

    for (from = 0, last = asteroid->edges - 1; from < last; from++) {
        next = from + 1;

        SDL_RenderDrawLine(
            renderer,
            points[from].x, points[from].y,
            points[next].x, points[next].y);
    }

    // close
    SDL_RenderDrawLine(
        renderer,
        points[last].x, points[last].y,
        points[0].x, points[0].y);
}
