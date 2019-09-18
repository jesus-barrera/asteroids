#include <stdlib.h>
#include "text.h"
#include "game.h"

Text *text_create(char *ctext, int x, int y, int ptsize)
{
    Text *text = (Text *)malloc(sizeof(Text));

    text->ptsize = ptsize;

    text_set_text(text, ctext);

    text_set_x(text, x);
    text_set_y(text, y);

    return text;
}

void text_destroy(Text *text)
{
    SDL_DestroyTexture(text->texture);
    free(text);
}

void text_update_size(Text *text)
{
    int w, h;

    SDL_QueryTexture(text->texture, NULL, NULL, &w, &h);

    text->rect.w = w;
    text->rect.h = h;
}

void text_set_text(Text *text, char *ctext)
{
    // remove previous texture
    if (text->texture) {
        SDL_DestroyTexture(text->texture);
    }

    // create texture and set text
    text->texture = text_load_texture(ctext, text->ptsize);
    text->text = ctext;

    text_update_size(text);
}

void text_set_x(Text *text, int x)
{
    if (x == TEXT_ALIGN_CENTER) {
        x = WINDOW_WIDTH / 2 - text->rect.w / 2;
    }

    text->rect.x = x;
}

void text_set_y(Text *text, int y)
{
    if (y == TEXT_ALIGN_CENTER) {
        y = WINDOW_HEIGHT / 2 - text->rect.h / 2;
    }

    text->rect.y = y;
}

void text_render(Text *text)
{
    SDL_RenderCopy(renderer, text->texture, NULL, &text->rect);
}

SDL_Texture *text_load_texture(const char*text, int ptsize)
{
    TTF_Font *font;
    SDL_Color color = {0xFF, 0xFF, 0xFF};
    SDL_Texture* text_texture = NULL;
    SDL_Surface* text_surface;

    // load font
    font = TTF_OpenFont("media/fonts/hyperspace/Hyperspace.otf", ptsize);

    // render text
    if (text_surface = TTF_RenderText_Solid(font, text, color)) {
        text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_FreeSurface(text_surface);
    }

    TTF_CloseFont(font);

    return text_texture;
}
