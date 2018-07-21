#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>

#include "game.h"
#include "timer.h"

SDL_bool init();
void handle_event(SDL_Event *event);
void render();
void clean();

SDL_Window *window = NULL;
SDL_bool quit;

Scene *current_scene = NULL;

int main(int argc, char *argv[])
{
    SDL_Event event;
    Timer game_timer;

    if (! init()) {
        return 0;
    }

    timer_start(&game_timer);

    current_scene = &game;
    current_scene->enter();

    quit = SDL_FALSE;

    while (! quit) {
        while (SDL_PollEvent(&event)) {
            handle_event(&event);
        }

        // get time from last update
        time_step = timer_get_seconds(&game_timer);

        // update scene
        current_scene->update();

        // restart timer
        timer_start(&game_timer);

        render();
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
                "Asteroids",
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

    return SDL_TRUE;
}

/**
 * Handle SDL envent.
 */
void handle_event(SDL_Event *event)
{
    if (event->type == SDL_QUIT) {
        quit = SDL_TRUE;
    } else {
        current_scene->handle_event(event);
    }
}

void render()
{
    // clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // render scene
    current_scene->render();
    SDL_RenderPresent(renderer);
}

void clean()
{
    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}
