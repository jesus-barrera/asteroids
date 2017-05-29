#include "game.h"
#include "game_scene.h"
#include "spaceship.h"
#include "asteroid.h"
#include "bullet.h"
#include "list.h"
#include "geometry.h"

void handle_keys();
void shoot();
void update_bullets();
SDL_bool check_bullet_collision(Bullet *bullet);
void draw_bullets(SDL_Renderer *renderer);
void update_asteroids();
void draw_asteroids(SDL_Renderer *renderer);

Scene game_scene = {enter, update, render, handle_event};

Node *asteroids;
Node *bullets;

Spaceship *ship;

void enter()
{
    Asteroid *asteroid;
    int i;

    // create asteroids
    for (i = 0; i < NUM_ASTEROIDS; i++) {
        asteroid = new_asteroid(
                uniform(18, 50), 18,
                uniform(0, game_viewport.w), uniform(0, game_viewport.h),
                uniform(0, 2 * PI),
                uniform(1, 2));

        list_append(&asteroids, (void *)asteroid);
    }

    // create a spaceship at the center of the screen pointing up.
    ship = new_spaceship(game_viewport.w / 2, game_viewport.h / 2, 3 * PI / 2, 0);
}

void update()
{
    move_spaceship(ship);
    update_bullets();
    update_asteroids();

    handle_keys();
}

void render(SDL_Renderer *renderer)
{
    SDL_RenderSetViewport(renderer, &game_viewport);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    draw_spaceship(ship, renderer);
    draw_bullets(renderer);
    draw_asteroids(renderer);
}

void handle_event(SDL_Event *event)
{
    SDL_Keycode key;

    if (event->type == SDL_KEYDOWN && ! event->key.repeat) {
        key = event->key.keysym.sym;

        if (key == SDLK_SPACE) {
            shoot();
        }
    }
}

void handle_keys()
{
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    if (keystates[SDL_SCANCODE_LEFT]) {
        rotate_spaceship(ship, SHIP_ANGLE_UPDATE * time_step * -1);
    } else if (keystates[SDL_SCANCODE_RIGHT]) {
        rotate_spaceship(ship, SHIP_ANGLE_UPDATE * time_step);
    } else if (keystates[SDL_SCANCODE_UP]) {
        add_object_velocity(&ship->obj, SHIP_VELOCITY_UPDATE * time_step);
    }
}

void shoot()
{
    float height;
    Bullet *bullet;

    bullet = new_bullet(
        ship->vertices[SHIP_TOP_VERTEX].x,
        ship->vertices[SHIP_TOP_VERTEX].y,
        ship->obj.angle,
        BULLET_VELOCITY);

    list_append(&bullets, (void *)bullet);
}

void update_bullets()
{
    Node **node_ref;
    Bullet *bullet;

    node_ref = &bullets;

    while (*node_ref != NULL) {
        bullet = (Bullet *)(*node_ref)->data;
        move_bullet(bullet);

        if (check_bullet_collision(bullet)) {
            delete_bullet(bullet);
            *node_ref = destroy_node(*node_ref);
        } else {
            node_ref = &(*node_ref)->next;
        }
    }
}

SDL_bool check_bullet_collision(Bullet *bullet)
{
    Node **node_ref = &asteroids;
    Asteroid *asteroid;

    Point bullet_line[2];

    if (is_off_screen(&bullet->obj)) {
        return SDL_TRUE;
    }

    bullet_line[0] = bullet->obj.position;
    bullet_line[1] = bullet->end;

    // check asteroid collision
    while (*node_ref != NULL) {
        asteroid = (Asteroid *)(*node_ref)->data;

        if (polygon_intersect_line(&asteroid->polygon, bullet_line)) {
            *node_ref = destroy_node(*node_ref);

            return SDL_TRUE;
        } else {
            node_ref = &(*node_ref)->next;
        }
    }

    return SDL_FALSE;
}

void draw_bullets(SDL_Renderer *renderer)
{
    Node *node = bullets;

    while (node != NULL) {
        draw_bullet((Bullet *)node->data, renderer);
        node = node->next;
    }
}

void update_asteroids()
{
    Node **node_ref;
    Asteroid *asteroid;

    node_ref = &asteroids;

    while (*node_ref != NULL) {
        asteroid = (Asteroid *)(*node_ref)->data;
        move_asteroid(asteroid);
        node_ref = &(*node_ref)->next;
    }
}

void draw_asteroids(SDL_Renderer *renderer)
{
    Node *node = asteroids;

    while (node != NULL) {
        draw_asteroid((Asteroid *)node->data, renderer);
        node = node->next;
    }
}
