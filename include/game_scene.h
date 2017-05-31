#ifndef _GAME_SCENE_H_INCLUDED_
#define _GAME_SCENE_H_INCLUDED_

#include "scene.h"

#define NUM_ASTEROIDS 7

#define SHIP_MAX_VELOCITY 200 // pixels per second
#define SHIP_ACCELERATION 200 // pixels per second^2
#define SHIP_ANGLE_UPDATE PI  // radians per second
#define BULLET_VELOCITY 200   // pixels per second

extern Scene game_scene;

static void enter();
static void update();
static void render(SDL_Renderer *renderer);
static void handle_event(SDL_Event *event);

#endif
