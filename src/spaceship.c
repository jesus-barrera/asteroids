#include <math.h>
#include "spaceship.h"
#include "game.h"

void set_spaceship_vertices(Spaceship *ship);

Spaceship *new_spaceship(int x, int y, float angle, float velocity)
{
    Spaceship *ship = (Spaceship *)malloc(sizeof(Spaceship));

    ship->obj.position.x = x;
    ship->obj.position.y = y;
    ship->obj.angle = angle;
    set_object_velocity(&ship->obj, velocity);

    ship->edges = 3;
    ship->vertices = (Point *)malloc(sizeof(Point) * ship->edges);

    return ship;
}

void delete_spaceship(Spaceship *ship)
{
    free(ship->vertices);
    free(ship);
}

void move_spaceship(Spaceship *ship)
{
    move_object(&ship->obj);
}

void draw_spaceship(Spaceship *ship, SDL_Renderer *renderer)
{
    set_spaceship_vertices(ship);
    draw_polygon(renderer, ship->vertices, ship->edges);
}

void set_spaceship_vertices(Spaceship *ship)
{
    Point *vertices;
    float height, base, hypotenuse, angle, aux_angle;

    vertices = ship->vertices;

    height = SPACESHIP_HEIGHT / (float)2;
    base = SPACESHIP_BASE / (float)2;

    // top vertex
    vertices[0].x = ship->obj.position.x + cos(ship->obj.angle) * height;
    vertices[0].y = ship->obj.position.y + sin(ship->obj.angle) * height;

    // base vertices
    hypotenuse = sqrt(pow(height, 2) + pow(base, 2));
    angle = asin(base / height);

    aux_angle = ship->obj.angle + PI - angle;
    vertices[1].x = ship->obj.position.x + cos(aux_angle) * hypotenuse;
    vertices[1].y = ship->obj.position.y + sin(aux_angle) * hypotenuse;

    aux_angle = ship->obj.angle + PI + angle;
    vertices[2].x = ship->obj.position.x + cos(aux_angle) * hypotenuse;
    vertices[2].y = ship->obj.position.y + sin(aux_angle) * hypotenuse;
}
