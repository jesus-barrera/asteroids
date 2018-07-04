#include <stdlib.h>
#include <math.h>
#include "bullet.h"

void set_bullet_end(Bullet *bullet);

Bullet *new_bullet(int x, int y, float angle, float velocity)
{
    Bullet *bullet = (Bullet *)malloc(sizeof(Bullet));

    set_object_props(&bullet->obj, x, y, angle, velocity);
    set_bullet_end(bullet);

    // initialize prev position as the current position
    bullet->prev_position = bullet->obj.position;

    return bullet;
}

void delete_bullet(Bullet *bullet)
{
    free(bullet);
}

void move_bullet(Bullet *bullet)
{
    // Save bullet position before update to trace the bullet movement. Collision is checked across
    // this trace instead the bullet itself, preventing it to pass through something without colliding.
    bullet->prev_position = bullet->obj.position;

    update_object_position(&bullet->obj);
    set_bullet_end(bullet);
}

void draw_bullet(Bullet *bullet, SDL_Renderer *renderer)
{
    SDL_RenderDrawLine(
        renderer,
        bullet->obj.position.x, bullet->obj.position.y,
        bullet->end.x, bullet->end.y);
}

void set_bullet_end(Bullet *bullet)
{
    bullet->end.x = bullet->obj.position.x + cos(bullet->obj.angle) * BULLET_LENGTH;
    bullet->end.y = bullet->obj.position.y + sin(bullet->obj.angle) * BULLET_LENGTH;
}
