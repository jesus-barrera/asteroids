#ifndef _BULLET_H_INCLUDED_
#define _BULLET_H_INCLUDED_

#include "object.h"

#define BULLET_LENGTH 10

typedef struct Bullet Bullet;

struct Bullet
{
    Object obj;
    Point end;
};

Bullet *new_bullet(int x, int y, float angle, float velocity);
void delete_bullet(Bullet *bullet);
void draw_bullet(Bullet *bullet, SDL_Renderer *renderer);

#endif
