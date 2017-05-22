#include "SDL.h"

#include "game_scene.h"

#define MAIN_WIN_HEIGHT 400
#define MAIN_WIN_WIDTH 600

SDL_bool init();
void handleEvent(SDL_Event *event);
void clean();

SDL_Rect info_viewport = {0,  0, MAIN_WIN_WIDTH, 80};
SDL_Rect game_viewport = {0, 80, MAIN_WIN_WIDTH, MAIN_WIN_HEIGHT - 80};

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
    quit = SDL_FALSE;

    while (quit == SDL_FALSE) {
        while (SDL_PollEvent(&event)) {
            handleEvent(&event);
        }

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        SDL_Delay(100);
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
                MAIN_WIN_WIDTH, MAIN_WIN_HEIGHT,
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
