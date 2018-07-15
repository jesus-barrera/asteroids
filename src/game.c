#include "game.h"

SDL_Rect info_viewport;
SDL_Rect game_viewport;

float time_step = 1;

SDL_Renderer *renderer = NULL;

void set_viewports()
{
    info_viewport.x = 0;
    info_viewport.y = 0;
    info_viewport.w = WINDOW_WIDTH;
    info_viewport.h = WINDOW_HEIGHT * 0.15;

    game_viewport.x = 0;
    game_viewport.y = info_viewport.h;
    game_viewport.w = WINDOW_WIDTH;
    game_viewport.h = WINDOW_HEIGHT - game_viewport.y;
}
