#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "game.h"
#include "title.h"
#include "timer.h"

SDL_bool init();
void handle_event(SDL_Event *event);
void render();
void load_assets();
void close();

SDL_Window *window;
SDL_bool quit;
SDL_Renderer *renderer;
Mix_Chunk *sounds[SFX_COUNT];
TTF_Font *font;
float time_step;

int main(int argc, char *argv[])
{
    SDL_Event event;
    Timer game_timer;

    if (! init()) {
        return 0;
    }

    load_assets();
    scene_load(&title);
    timer_start(&game_timer);

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

    close();

    return 0;
}

/**
 * Initialize SDL subsystems and create main objects.
 */
SDL_bool init()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Couldn't initialize SDL: %s \n", SDL_GetError());
        return SDL_FALSE;
    }

    // Initialize SDL_mixer: 22.05KHz, system byte order, stereo audio, using
    // 1024 byte chunks.
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) < 0 ) {
        printf("Couldn't initialize SDL_mixer: %s\n", Mix_GetError());

        return SDL_FALSE;
    }

    // Initialize SDL_ttf
    if ( TTF_Init() == -1 ) {
        printf( "Could not initialize SDL_ttf: %s\n", TTF_GetError() );

        return SDL_FALSE;
    }

    // Create Window
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

    // Create Renderer
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

void load_assets() {
    // load sounds
    sounds[SFX_FIRE] = Mix_LoadWAV("media/sound/fire.wav");
    sounds[SFX_EXPLOSION_LARGE] = Mix_LoadWAV("media/sound/bangLarge.wav");
    sounds[SFX_EXPLOSION_MEDIUM] = Mix_LoadWAV("media/sound/bangMedium.wav");
    sounds[SFX_EXPLOSION_SMALL] = Mix_LoadWAV("media/sound/bangSmall.wav");
    sounds[SFX_THRUST] = Mix_LoadWAV("media/sound/thrust.wav");

    // load font
    font = TTF_OpenFont("media/fonts/hyperspace/Hyperspace.otf", FONT_PTSIZE);
}

void close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_CloseFont(font);

    window = NULL;
    renderer = NULL;
    font = NULL;

    // Quit SDL subsystems
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}
