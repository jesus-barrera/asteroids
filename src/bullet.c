#include "bullet.h"

void set_bullet_end(Bullet *bullet);

Bullet *new_bullet(int x, int y, float angle, float velocity)
{
    Bullet *bullet = (Bullet *)malloc(sizeof(Bullet));

    set_object_props(&bullet->obj, x, y, angle, velocity);
    set_bullet_end(bullet);

    return bullet;
}

void delete_bullet(Bullet *bullet)
{
    free(bullet);
}

void move_bullet(Bullet *bullet)
{
    move_object(&bullet->obj);
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
