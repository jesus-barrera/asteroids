#include <SDL.h>
#include "title.h"
#include "text.h"
#include "game.h"
#include "tools.h"
#include "asteroid.h"
#include "list.h"
#include <stdio.h>

static void init();
static void update();
static void render();
static void handle_event(SDL_Event*);
static void quit();

static Text *title_text;
static Text *press_text;

static Node *asteroids;

Scene title = {init, update, render, handle_event, quit};

void init()
{
    title_text = text_create(
        "ASTEROIDS",
        TEXT_ALIGN_CENTER, TITLE_TITLE_Y_OFFSET,
        TITLE_TITLE_PTSIZE);

    press_text = text_create(
        "Presiona una tecla para empezar",
        TEXT_ALIGN_CENTER, TITLE_SUBTITLE_Y_OFFSET,
        TITLE_SUBTITLE_PTSIZE);

    asteroids = asteroid_create_many(TITLE_NUM_OF_ASTEROIDS);
}

void update()
{
    objects_update(asteroids);
}

void render()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    objects_draw(asteroids);

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

    while (asteroids) {
        asteroid_delete((Asteroid *)asteroids->data);
        asteroids = destroy_node(asteroids);
    }
}
