#include <stdlib.h>
#include <math.h>
#include "bullet.h"
#include "game.h"

void bullet_set_points(Bullet *bullet);

Bullet *bullet_new(int x, int y, float direction, float speed)
{
    Bullet *bullet = (Bullet *)object_new(x, y,
                                          BULLET_LENGTH / 2.0, direction,
                                          speed, 2);

    bullet_set_points(bullet);

    return bullet;
}

void bullet_move(Bullet *bullet)
{
    object_update_position((Object *)bullet);
    bullet_set_points(bullet);
}

void bullet_draw(Bullet *bullet)
{
    SDL_RenderDrawLine(
        renderer,
        bullet->points[0].x, bullet->points[0].y,
        bullet->points[1].x, bullet->points[1].y);
}

void bullet_set_points(Bullet *bullet)
{
    object_set_point((Object *)bullet, 0, 0);
    object_set_point((Object *)bullet, 1, PI);
}
