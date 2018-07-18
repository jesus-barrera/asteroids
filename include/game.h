#ifndef _GAME_H_INCLUDED_
#define _GAME_H_INCLUDED_

#include <SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define SPACESHIP_MAX_SPEED 300        // pixels per second
#define SPACESHIP_ACCELERATION 0.9     // (?)
#define SPACESHIP_DECELERATION 0.3     // (?)
#define SPACESHIP_ANGULAR_VELOCITY PI  // radians per second

#define BULLET_SPEED 400               // pixels per second

extern SDL_Rect info_viewport;
extern SDL_Rect game_viewport;
extern SDL_Renderer *renderer;

extern float time_step;

void set_viewports();

#endif
