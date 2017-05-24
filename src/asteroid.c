#include <math.h>
#include "game.h"
#include "asteroid.h"

void set_asteroid_vertices(Asteroid *asteroid, int edges);

Asteroid *new_asteroid(float radius, int edges, int x, int y, float angle, float velocity)
{
    Asteroid *asteroid = (Asteroid *)malloc(sizeof(Asteroid));

    asteroid->radius = radius;
    set_object_props((Object *)asteroid, x, y, angle, velocity);
    set_asteroid_vertices(asteroid, edges);

    return asteroid;
}

void delete_asteroid(Asteroid *asteroid)
{
    free(asteroid->polygon.vertices);
    free(asteroid);
}

void move_asteroid(Asteroid *asteroid)
{
    move_polygon((Polygon *)asteroid);
}

void draw_asteroid(Asteroid *asteroid, SDL_Renderer *renderer)
{
    draw_polygon((Polygon *)asteroid, renderer);
}

void set_asteroid_vertices(Asteroid *asteroid, int edges)
{
    int i;
    float angle, angle_step, angle_min, angle_max, radius;
    Polygon *polygon;
    Object *obj;

    polygon = (Polygon *)asteroid;
    obj = (Object *)polygon;

    polygon->edges = edges;
    polygon->vertices = (Point *)malloc(sizeof(Point) * edges);

    angle_step = 2 * PI / polygon->edges;
    angle_min = 0;

    for (i = 0; i < edges; i++) {
        angle_max = angle_min + angle_step;

        radius = uniform(asteroid->radius * MIN_RADIUS, asteroid->radius);
        angle = uniform(angle_min, angle_max);

        polygon->vertices[i].x = obj->position.x + cos(angle) * radius;
        polygon->vertices[i].y = obj->position.y + sin(angle) * radius;

        angle_min = angle_max;
    }
}
