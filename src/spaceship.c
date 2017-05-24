#include <math.h>
#include "spaceship.h"
#include "game.h"

enum { TOP, BOTTOM_LEFT, BOTTOM_RIGHT, NUM_VERTICES };

void set_spaceship_vertices(Spaceship *ship);
void set_vertex(Point *vertex, Point *origin, float angle, float magnitude);

Spaceship *new_spaceship(int x, int y, float angle, float velocity)
{
    Spaceship *ship = (Spaceship *)malloc(sizeof(Spaceship));

    set_object_props(&ship->obj, x, y, angle, velocity);

    ship->edges = NUM_VERTICES;
    ship->vertices = (Point *)malloc(sizeof(Point) * ship->edges);

    set_spaceship_vertices(ship);

    return ship;
}

void delete_spaceship(Spaceship *ship)
{
    free(ship->vertices);
    free(ship);
}

void move_spaceship(Spaceship *ship)
{
    move_polygon(ship);
}

void rotate_spaceship(Spaceship *ship, float angle)
{
    ship->obj.angle += angle;
    set_spaceship_vertices(ship);
}

void draw_spaceship(Spaceship *ship, SDL_Renderer *renderer)
{
    draw_polygon(ship, renderer);
}

void set_spaceship_vertices(Spaceship *ship)
{
    Point *vertices, *origin;
    float height, base, hypotenuse, angle, aux_angle;

    vertices = ship->vertices;
    origin = &ship->obj.position;

    height = SPACESHIP_HEIGHT / (float)2;
    base = SPACESHIP_BASE / (float)2;

    // top vertex
    set_vertex(&vertices[TOP], origin, ship->obj.angle, height);

    // base vertices
    hypotenuse = sqrt(pow(height, 2) + pow(base, 2));
    angle = asin(base / height);

    set_vertex(&vertices[BOTTOM_LEFT], origin, ship->obj.angle + PI - angle, hypotenuse);
    set_vertex(&vertices[BOTTOM_RIGHT], origin, ship->obj.angle + PI + angle, hypotenuse);
}

void set_vertex(Point *vertex, Point *origin, float angle, float magnitude)
{
    vertex->x = origin->x + cos(angle) * magnitude;
    vertex->y = origin->y + sin(angle) * magnitude;
}
