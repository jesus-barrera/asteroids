#ifndef _GAME_H_INCLUDED_
#define _GAME_H_INCLUDED_

#include <SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

extern SDL_Rect info_viewport;
extern SDL_Rect game_viewport;

extern float time_step;

void set_viewports();

#endif
