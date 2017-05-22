#include "game_scene.h"

Scene game_scene = {enter, update, render, handleEvent};

void enter()
{

}

void update()
{

}

void render()
{

}

void handleEvent(SDL_Event *event)
{
    if (event->type == SDL_KEYDOWN) {
        SDL_Keycode key = event->key.keysym.sym;

        switch (key) {
            case SDLK_UP:
                break;
            case SDLK_LEFT:
                break;
            case SDLK_RIGHT:
                break;
            case SDLK_SPACE:
                break;
            default:
                // nothing to do
                break;
        }
    }
}
