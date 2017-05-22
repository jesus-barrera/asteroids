#ifndef _GAME_SCENE_H_INCLUDED_
#define _GAME_SCENE_H_INCLUDED_

#include "scene.h"

extern Scene game_scene;

static void enter();
static void update();
static void render();
static void handleEvent(SDL_Event *event);
static void render();

#endif
