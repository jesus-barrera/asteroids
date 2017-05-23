#include <math.h>
#include "spaceship.h"
#include "game.h"

Spaceship *new_spaceship(int x, int y)
{
    Spaceship *ship = (Spaceship *)malloc(sizeof(Spaceship));

    ship->position.x = x;
    ship->position.y = y;

    return ship;
}

void delete_spaceship(Spaceship *ship)
{
    free(ship);
}

void move_spaceship(Spaceship *ship)
{
    // set new position
    ship->position.x += ship->x_velocity * time_step;
    ship->position.y += ship->y_velocity * time_step;

    // detect edge collision
    if (ship->position.x > game_viewport.w) {
        ship->position.x = game_viewport.x;
    } else if (ship->position.x < 0) {
        ship->position.x = game_viewport.w;
    }

    if (ship->position.y > game_viewport.h) {
        ship->position.y = 0;
    } else if (ship->position.y < 0) {
        ship->position.y = game_viewport.h;
    }
}

void draw_spaceship(Spaceship *ship, SDL_Renderer *renderer)
{
    Point points[3]; // triangle
    float height, width, hypotenuse, angle;

    height = SPACESHIP_HEIGHT / (float)2;
    width = SPACESHIP_WIDTH / (float)2;

    // top point
    points[0].x = ship->position.x + cos(ship->angle) * height;
    points[0].y = ship->position.y + sin(ship->angle) * height;

    // bottom points
    hypotenuse = sqrt(pow(height, 2) + pow(width, 2));
    angle = asin(width / height);

    points[1].x = ship->position.x + cos(ship->angle + PI - angle) * hypotenuse;
    points[1].y = ship->position.y + sin(ship->angle + PI - angle) * hypotenuse;

    points[2].x = ship->position.x + cos(ship->angle + PI + angle) * hypotenuse;
    points[2].y = ship->position.y + sin(ship->angle + PI + angle) * hypotenuse;

    draw_polygon(renderer, points, 3);
}
