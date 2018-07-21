#ifndef _SCENE_H_INCLUDED_
#define _SCENE_H_INCLUDED_

#include <SDL.h>

struct Scene {
    void (*enter)(void);
    void (*update)(void);
    void (*render)();
    void (*handle_event)(SDL_Event *);
};

typedef struct Scene Scene;

#endif
