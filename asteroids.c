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

    current_scene = &game_scene;
    current_scene->enter();

    quit = SDL_FALSE;

    while (quit == SDL_FALSE) {
        while (SDL_PollEvent(&event)) {
            handleEvent(&event);
        }

        // clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        // render scene
        current_scene->render(renderer);
        SDL_RenderPresent(renderer);

        // update scene
        current_scene->update();

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
