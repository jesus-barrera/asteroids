#ifndef _BULLET_H_INCLUDED_
#define _BULLET_H_INCLUDED_

#include "object.h"

#define BULLET_LENGTH 2

#define bullet_delete(bullet) object_delete((Object *)bullet)
#define bullet_draw(bullet) object_draw((Object *)bullet)

typedef struct Object Bullet;

Bullet *bullet_new(int x, int y, float direction, float speed);
void bullet_move(Bullet *bullet);

#endif
