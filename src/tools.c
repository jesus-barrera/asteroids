#include <stdlib.h>
#include "game.h"
#include "tools.h"

float uniform(float min, float max)
{
    return (rand() / (float)RAND_MAX) * (max - min) + min;
}

int randint(int min, int max)
{
    return rand() % (max - min) + min;
}

SDL_Texture *create_text_texture(const char *text)
{
    SDL_Color color = {0xFF, 0xFF, 0xFF}; // use white by default
    SDL_Texture* text_texture = NULL;
    SDL_Surface* text_surface;

    if (text_surface = TTF_RenderText_Solid(font, text, color)) {
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_FreeSurface(text_surface);
    }

    return text_texture;
}
