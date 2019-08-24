#ifndef _SPACESHIP_H_INCLUDED_
#define _SPACESHIP_H_INCLUDED_

#include "timer.h"
#include "object.h"

#define SPACESHIP_RADIUS 10
#define SPACESHIP_POINTS_COUNT 5
#define SPACESHIP_FLAME_POINTS_COUNT 3
#define SPACESHIP_TOTAL_POINTS_COUNT SPACESHIP_POINTS_COUNT + SPACESHIP_FLAME_POINTS_COUNT

#define SPACESHIP_TOP_VERTEX 0

#define SPACESHIP_STATE_OK         0x00000001
#define SPACESHIP_STATE_THRUST     0x00000010
#define SPACESHIP_STATE_DESTROYED  0x00000100
#define SPACESHIP_STATE_INVINCIBLE 0x00001000

typedef struct {
    Object obj;
    int state;
    Timer flickering_timer;
    Timer state_timer;
    SDL_bool draw_flame;
    Node *parts;
} Spaceship;

Spaceship *spaceship_new(int x, int y, float direction, float speed);
void spaceship_delete(Spaceship *ship);
void spaceship_update(Spaceship *ship);
void spaceship_draw(Spaceship *ship);
void spaceship_destroy(Spaceship *ship);
void spaceship_update_velocity(Spaceship *ship, float target_speed, float acceleration);

#endif
