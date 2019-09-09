/**
 * Scenes handle the updating, rendering and events within a particular screen
 * of the game, such the title screen or the in-game screen.
 */

#ifndef _SCENE_H_INCLUDED_
#define _SCENE_H_INCLUDED_

#include <SDL.h>

struct Scene {
    void (*init)();
    void (*update)();
    void (*render)();
    void (*handle_event)(SDL_Event *);
    void (*quit)();
};

typedef struct Scene Scene;

/**
 * Sets the current scene.
 */
void scene_load(Scene *scene);

/**
 * Current running scene.
 */
extern Scene *current_scene;

#endif
