#include <SDL.h>
#include "title.h"
#include "game.h"
#include "tools.h"

static void init();
static void update();
static void render();
static void handle_event(SDL_Event*);
static void quit();

SDL_Texture* title_text;
Scene title = {init, update, render, handle_event, quit};

void init()
{
    title_text = create_text_texture("ASTEROIDS");
}

void update()
{

}

void render()
{
    SDL_RenderCopy(renderer, title_text, NULL, NULL);
}

void handle_event(SDL_Event *event)
{
    SDL_Keycode key;

    if (event->type == SDL_KEYDOWN) {
        scene_load(&game);
    }
}

void quit()
{
    SDL_DestroyTexture(title_text);
    title_text = NULL;
}
