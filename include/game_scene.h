#ifndef _GAME_SCENE_H_INCLUDED_
#define _GAME_SCENE_H_INCLUDED_

#include "scene.h"

extern Scene game_scene;

static void enter();
static void update();
static void render(SDL_Renderer *renderer);
static void handle_event(SDL_Event *event);

#endif
