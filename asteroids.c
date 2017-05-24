#include "SDL.h"

#include "game.h"
#include "game_scene.h"
#include "asteroid.h"
#include "spaceship.h"

SDL_bool init();
void handleEvent(SDL_Event *event);
void clean();

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_bool quit;

Scene *current_scene = NULL;

int main(int argc, char *argv[])
{
    SDL_Event event;

    if (! init()) {
        return 0;
    }

    Asteroid *asteroid = new_asteroid(50, 18, 200, 200);
    asteroid->velocity = 5;
    asteroid->angle = 1;

    Spaceship *ship = new_spaceship(10, 10);
    ship->x_velocity = 1;

    time_step = 1;

    current_scene = &game_scene;
    quit = SDL_FALSE;

    SDL_RenderSetViewport(renderer, &game_viewport);

    while (quit == SDL_FALSE) {
        while (SDL_PollEvent(&event)) {
            handleEvent(&event);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

        draw_asteroid(asteroid, renderer);
        draw_spaceship(ship, renderer);
        SDL_RenderPresent(renderer);

        move_asteroid(asteroid);
        move_spaceship(ship);

        SDL_Delay(10);
    }

    clean();

    return 0;
}


/**
 * Initialize SDL subsystems and create main objects.
 */
SDL_bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Couldn't initialize SDL video: %s \n", SDL_GetError());
        return SDL_FALSE;
    }

    window = SDL_CreateWindow(
                "Asteroids Game",
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                WINDOW_WIDTH, WINDOW_HEIGHT,
                SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("Couldn't create window: %s \n", SDL_GetError());
        return SDL_FALSE;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        printf("Couldn't create renderer: %s \n", SDL_GetError());
        return SDL_FALSE;
    }

    // game things
    srand(time(NULL)); // initialize rand module
    set_viewports();

    return SDL_TRUE;
}

/**
 * Handle SDL envent.
 */
void handleEvent(SDL_Event *event)
{
    if (event->type == SDL_QUIT) {
        quit = SDL_TRUE;
    } else {
        current_scene->handleEvent(event);
    }
}

void clean()
{
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}
