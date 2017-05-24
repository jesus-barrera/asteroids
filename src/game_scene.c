#include "game.h"
#include "game_scene.h"
#include "spaceship.h"
#include "asteroid.h"

#define NUM_ASTEROIDS 7
#define SHIP_VELOCITY_UPDATE 0.05
#define SHIP_ANGLE_UPDATE 0.05

void handleKeys();

Scene game_scene = {enter, update, render, handleEvent};

Asteroid *asteroids[NUM_ASTEROIDS];
Spaceship *ship;

void enter()
{
    int i;

    for (i = 0; i < NUM_ASTEROIDS; i++) {
        asteroids[i] = new_asteroid(
                uniform(18, 50), 18,
                uniform(0, game_viewport.w), uniform(0, game_viewport.h),
                uniform(0, 2 * PI),
                uniform(1, 2));
    }

    ship = new_spaceship(game_viewport.w / 2, game_viewport.h / 2, -PI / 2, 0);
}

void update()
{
    int i;

    for (i = 0; i < NUM_ASTEROIDS; i++) {
        move_asteroid(asteroids[i]);
    }

    move_spaceship(ship);

    handleKeys();
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

void handleEvent(SDL_Event *event)
{

}

void check_collisions()
{

}

void handleKeys()
{
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_LEFT]) {
        ship->obj.angle -= SHIP_ANGLE_UPDATE * time_step;
    } else if (keystates[SDL_SCANCODE_RIGHT]) {
        ship->obj.angle += SHIP_ANGLE_UPDATE * time_step;
    } else if (keystates[SDL_SCANCODE_UP]) {
        add_object_velocity(&ship->obj, SHIP_VELOCITY_UPDATE * time_step);
    }
}
