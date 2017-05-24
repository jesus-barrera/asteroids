#include "bullet.h"

void set_bullet_end(Bullet *bullet);

Bullet *new_bullet(int x, int y, float angle, float velocity)
{
    Bullet *bullet = (Bullet *)malloc(sizeof(Bullet));

    bullet->shot = SDL_FALSE;

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
    if (bullet->shot) {
        // update positions
        update_object_position(&bullet->obj);

        if (is_off_screen(&bullet->obj)) {
            bullet->shot = SDL_FALSE;
        }
    }
}

void draw_bullet(Bullet *bullet, SDL_Renderer *renderer)
{
    if (bullet->shot) {
        set_bullet_end(bullet);

        SDL_RenderDrawLine(
            renderer,
            bullet->obj.position.x, bullet->obj.position.y,
            bullet->end.x, bullet->end.y);
    }
}

void set_bullet_end(Bullet *bullet)
{
    bullet->end.x = bullet->obj.position.x + cos(bullet->obj.angle) * BULLET_LENGTH;
    bullet->end.y = bullet->obj.position.y + sin(bullet->obj.angle) * BULLET_LENGTH;
}
