#include "game.h"
#include "game_scene.h"
#include "spaceship.h"
#include "asteroid.h"
#include "bullet.h"

#define NUM_ASTEROIDS 7
#define SHIP_VELOCITY_UPDATE 0.05
#define SHIP_ANGLE_UPDATE 0.05

void handleKeys();
void shoot();

Scene game_scene = {enter, update, render, handleEvent};

Asteroid *asteroids[NUM_ASTEROIDS];
Spaceship *ship;
Bullet *bullet;

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

    bullet = new_bullet(0, 0, 0, 0);
}

void update()
{
    int i;

    for (i = 0; i < NUM_ASTEROIDS; i++) {
        move_asteroid(asteroids[i]);
    }

    move_spaceship(ship);
    move_bullet(bullet);

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
    draw_bullet(bullet, renderer);
}

void handleEvent(SDL_Event *event)
{
    SDL_Keycode key;

    if (event->type == SDL_KEYDOWN) {
        key = event->key.keysym.sym;

        if (key == SDLK_SPACE) {
            shoot();
        }
    }
}

void handleKeys()
{
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_LEFT]) {
        rotate_spaceship(ship, SHIP_ANGLE_UPDATE * time_step * -1);
    } else if (keystates[SDL_SCANCODE_RIGHT]) {
        rotate_spaceship(ship, SHIP_ANGLE_UPDATE * time_step);
    } else if (keystates[SDL_SCANCODE_UP]) {
        add_object_velocity(&ship->obj, SHIP_VELOCITY_UPDATE * time_step);
    }
}

void shoot()
{
    if (! bullet->shot) {
        float height;

        height = SPACESHIP_HEIGHT / 2.0;

        // place bullet
        bullet->obj.position.x = ship->obj.position.x + cos(ship->obj.angle) * height;
        bullet->obj.position.y = ship->obj.position.y + sin(ship->obj.angle) * height;
        bullet->obj.angle = ship->obj.angle;

        bullet->shot = SDL_TRUE;

        set_object_velocity(&bullet->obj, 5);
    }
}
