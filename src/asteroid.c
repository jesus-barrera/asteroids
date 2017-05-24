#include <math.h>
#include "game.h"
#include "asteroid.h"

void set_asteroid_vetices(Asteroid *asteroid);

Asteroid *new_asteroid(float radius, int edges, int x, int y, float angle, float velocity)
{
    Asteroid *asteroid = (Asteroid *)malloc(sizeof(Asteroid));

    asteroid->radius = radius;
    asteroid->edges = edges;
    asteroid->obj.position.x = x;
    asteroid->obj.position.y = y;
    asteroid->obj.angle = angle; // important! set angle before velocity
    set_object_velocity(&(asteroid->obj), velocity);

    set_asteroid_vetices(asteroid);

    return asteroid;
}

void set_asteroid_vetices(Asteroid *asteroid)
{
    int i;
    float angle, angle_step, angle_min, angle_max, radius;

    asteroid->vertices = (Point *)malloc(sizeof(Point) * asteroid->edges);

    angle_step = 2 * PI / asteroid->edges;
    angle_min = 0;

    for (i = 0; i < asteroid->edges; i++) {
        angle_max = angle_min + angle_step;

        radius = uniform(asteroid->radius * MIN_RADIUS, asteroid->radius);
        angle = uniform(angle_min, angle_max);

        asteroid->vertices[i].x = asteroid->obj.position.x + cos(angle) * radius;
        asteroid->vertices[i].y = asteroid->obj.position.y + sin(angle) * radius;

        angle_min = angle_max;
    }
}

void delete_asteroid(Asteroid *asteroid)
{
    free(asteroid->vertices);
    free(asteroid);
}

void move_asteroid(Asteroid *asteroid)
{
    int i;
    float x_offset, y_offset;
    Point position;

    // save asteroid position
    position = asteroid->obj.position;

    move_object(&(asteroid->obj));

    // calculate asteroid displacement
    x_offset = asteroid->obj.position.x - position.x;
    y_offset = asteroid->obj.position.y - position.y;

    // move asteroid vertices
    for (i = 0; i < asteroid->edges; i++) {
        asteroid->vertices[i].x += x_offset;
        asteroid->vertices[i].y += y_offset;
    }
}

void draw_asteroid(Asteroid *asteroid, SDL_Renderer *renderer)
{
    draw_polygon(renderer, asteroid->vertices, asteroid->edges);
}
