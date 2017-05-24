#include "game.h"
#include "game_scene.h"
#include "spaceship.h"
#include "asteroid.h"

#define NUM_ASTEROIDS 7

Scene game_scene = {enter, update, render, handleEvent};

Asteroid *asteroids[NUM_ASTEROIDS];
Spaceship *ship;

void enter()
{
    int i;

    for (i = 0; i < NUM_ASTEROIDS; i++) {
        asteroids[i] = new_asteroid(uniform(18, 50), 18, uniform(0, game_viewport.w), uniform(0, game_viewport.h));
        asteroids[i]->velocity = uniform(1, 2);
        asteroids[i]->angle = uniform(0, 2 * PI);
    }

    ship = new_spaceship(game_viewport.w / 2, game_viewport.h / 2);
}

void update()
{
    int i;

    for (i = 0; i < NUM_ASTEROIDS; i++) {
        move_asteroid(asteroids[i]);
    }

    move_spaceship(ship);

    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_LEFT]) {
        ship->angle -= 0.05;
    } else if (keystates[SDL_SCANCODE_RIGHT]) {
        ship->angle += 0.05;
    } else if (keystates[SDL_SCANCODE_UP]) {
        ship->x_velocity += cos(ship->angle) * 0.05;
        ship->y_velocity += sin(ship->angle) * 0.05;
    }
}

void render(SDL_Renderer *renderer)
{
    int i;

    SDL_RenderSetViewport(renderer, &game_viewport);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    for (i = 0; i < NUM_ASTEROIDS; i++) {
        draw_asteroid(asteroids[i], renderer);
    }

    draw_spaceship(ship, renderer);
}

void check_collisions()
{

}

void handleEvent(SDL_Event *event)
{

}
