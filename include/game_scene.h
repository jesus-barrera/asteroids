#ifndef _GAME_SCENE_H_INCLUDED_
#define _GAME_SCENE_H_INCLUDED_

#include "scene.h"

#define NUM_ASTEROIDS 7
#define SHIP_VELOCITY_UPDATE 0.05
#define SHIP_ANGLE_UPDATE 0.05
#define BULLET_VELOCITY 5

extern Scene game_scene;

static void enter();
static void update();
static void render(SDL_Renderer *renderer);
static void handle_event(SDL_Event *event);

#endif
