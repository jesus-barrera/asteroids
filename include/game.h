#ifndef _GAME_H_INCLUDED_
#define _GAME_H_INCLUDED_

#include <SDL.h>
#include <SDL_mixer.h>
#include "scene.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define SPACESHIP_MAX_SPEED 300
#define SPACESHIP_ACCELERATION 0.9
#define SPACESHIP_DECELERATION 0.3
#define SPACESHIP_ANGULAR_VELOCITY 3 * PI / 2
#define SPACESHIP_FLAME_FLICKERING_TIME 0.05

#define BULLET_SPEED 400

#define SAUCER_SMALL 20
#define SAUCER_LARGE 50
#define SAUCER_SPEED 50

#define LEVEL_INITIAL_ASTEROIDS 4
#define LEVEL_ASTEROID_INCREMENT 2

#define GAME_INITIAL_LIVES 3
#define GAME_RESPAWNING_TIME 3
#define GAME_INVINCIBILITY_TIME 3

#define FONT_PTSIZE 16

// Sound effects
enum {
    SFX_FIRE,
    SFX_EXPLOSION_LARGE,
    SFX_EXPLOSION_MEDIUM,
    SFX_EXPLOSION_SMALL,
    SFX_THRUST,
    SFX_COUNT
};

#define SFX_THRUST_CHANNEL 0

extern Mix_Chunk *sounds[SFX_COUNT];
extern SDL_Renderer *renderer;
extern float time_step;
extern Scene game;

#endif
