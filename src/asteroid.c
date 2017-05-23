#include <math.h>
#include "asteroid.h"
#include "game.h"

static void set_asteroid_points(Asteroid *asteroid);

/**
 * Create a new asteroid.
 */
Asteroid *new_asteroid(float radius, int edges, int x_pos, int y_pos)
{
    Asteroid *asteroid = (Asteroid *)malloc(sizeof(Asteroid));

    asteroid->radius = radius;
    asteroid->edges = edges;
    asteroid->position.x = x_pos;
    asteroid->position.y = y_pos;
    asteroid->velocity = 0;
    asteroid->angle = 0;

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

    asteroid->points = (Point *)malloc(sizeof(Point) * asteroid->edges);

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
void move_asteroid(Asteroid *asteroid)
{
    int i;
    float x_step, y_step, distance;
    Point position;

    // save asteroid position
    position = asteroid->position;


    // caculate movement
    distance = asteroid->velocity * time_step;

    x_step = cos(asteroid->angle) * distance;
    y_step = sin(asteroid->angle) * distance;

    // set new asteroid position
    asteroid->position.x += x_step;
    asteroid->position.y += y_step;

    // detect edge collision
    if (asteroid->position.x > game_viewport.w) {
        asteroid->position.x = game_viewport.x;
    } else if (asteroid->position.x < 0) {
        asteroid->position.x = game_viewport.w;
    }

    if (asteroid->position.y > game_viewport.h) {
        asteroid->position.y = 0;
    } else if (asteroid->position.y < 0) {
        asteroid->position.y = game_viewport.h;
    }

    // calculate asteroid final displacement and move points
    x_step = asteroid->position.x - position.x;
    y_step = asteroid->position.y - position.y;

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
    draw_polygon(renderer, asteroid->points, asteroid->edges);
}
