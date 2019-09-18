#ifndef _TEXT_H_INCLUDED_
#define _TEXT_H_INCLUDED_

#include <SDL.h>
#include <SDL_ttf.h>

#define TEXT_ALIGN_CENTER -1

typedef struct Text
{
    char *text;
    int ptsize;
    SDL_Rect rect;
    SDL_Texture *texture;
} Text;

Text *text_create(char *text, int x, int y, int ptsize);
void text_destroy(Text *text);
void text_set_text(Text *text, char *ctext);
void text_set_x(Text *text, int x);
void text_set_y(Text *text, int y);
void text_render(Text *text);

SDL_Texture *text_load_texture(const char *text, int ptsize);

#endif
