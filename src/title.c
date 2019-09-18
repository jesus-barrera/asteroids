#include <SDL.h>
#include "text.h"
#include "title.h"
#include "game.h"
#include "tools.h"

#define TITLE_Y_OFFSET 100
#define TITLE_PTSIZE 100
#define SUBTITLE_Y_OFFSET (WINDOW_HEIGHT - 100)
#define SUBTITLE_PTSIZE 25

static void init();
static void update();
static void render();
static void handle_event(SDL_Event*);
static void quit();

Text *title_text;
Text *press_text;

Scene title = {init, update, render, handle_event, quit};

void init()
{
    title_text = text_create(
        "ASTEROIDS",
        TEXT_ALIGN_CENTER, TITLE_Y_OFFSET,
        TITLE_PTSIZE);

    press_text = text_create(
        "Presiona una tecla para empezar",
        TEXT_ALIGN_CENTER, SUBTITLE_Y_OFFSET,
        SUBTITLE_PTSIZE);
}

void update()
{

}

void render()
{
    text_render(title_text);
    text_render(press_text);
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
    text_destroy(title_text);
    text_destroy(press_text);

    press_text = NULL;
    title_text = NULL;
}
