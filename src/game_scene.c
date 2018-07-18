#include "game.h"
#include "game_scene.h"
#include "spaceship.h"
#include "asteroid.h"
#include "bullet.h"
#include "list.h"
#include "geometry.h"

void handle_keys();
void update_ship_velocity(float target_speed, float acceleration);
void shoot();
void update_bullets();
SDL_bool check_bullet_collision(Bullet *bullet, Point *prev_position);
void draw_bullets(SDL_Renderer *renderer);
void update_asteroids();
void draw_asteroids(SDL_Renderer *renderer);

Scene game_scene = {enter, update, render, handle_event};

enum {
    SPACESHIP_STATE_OK,
    SPACESHIP_STATE_DESTROYED
};

Node *asteroids;
Node *bullets;

Spaceship *ship;
Object *spaceship_parts[SPACESHIP_NUM_VERTICES];
int spaceship_state = SPACESHIP_STATE_OK;

void enter()
{
    Asteroid *asteroid;
    int i;

    // create asteroids
    for (i = 0; i < 7; i++) {
        asteroid = asteroid_new(
                uniform(0, game_viewport.w), uniform(0, game_viewport.h),
                uniform(18, 50),
                uniform(0, 2 * PI),
                60,
                5);

        list_append(&asteroids, (void *)asteroid);
    }

    // create a spaceship at the center of the screen pointing up.
    ship = spaceship_new(game_viewport.w / 2, game_viewport.h / 2, -PI/2, 0);
}

void update()
{
    if (spaceship_state == SPACESHIP_STATE_OK) {
        spaceship_update(ship);

        Node **node_ref;
        Asteroid *asteroid;

        node_ref = &asteroids;

        while (*node_ref != NULL) {
            asteroid = (Asteroid *)(*node_ref)->data;

            if (object_check_collision((Object *)asteroid, (Object *)ship)) {
                spaceship_destroy();
                break;
            }

            node_ref = &(*node_ref)->next;
        }
    } else if (spaceship_state == SPACESHIP_STATE_DESTROYED) {
        int i;

        for (i = 0; i < SPACESHIP_NUM_VERTICES; i++) {
            object_update(spaceship_parts[i]);
        }
    }

    update_bullets();
    update_asteroids();

    handle_keys();
}

spaceship_destroy()
{
    int i, j;
    Point p1, p2;
    float x, y;

    for (i = 0; i < ship->num_points; i++) {
        j = (i + 1) % ship->num_points;

        p1 = ship->points[i];
        p2 = ship->points[j];

        x = p1.x + (p2.x - p1.x) / 2;
        y = p1.y + (p2.y - p1.y) / 2;

        spaceship_parts[i] = object_new(
            x, y,
            0,
            uniform(0, PI * 2),
            uniform(10, 20),
            2);

        spaceship_parts[i]->points[0] = p1;
        spaceship_parts[i]->points[1] = p2;
        spaceship_parts[i]->angular_speed = uniform(PI / 4, PI);
    }

    spaceship_state = SPACESHIP_STATE_DESTROYED;
}

void render(SDL_Renderer *renderer)
{
    SDL_RenderSetViewport(renderer, &game_viewport);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    if (spaceship_state == SPACESHIP_STATE_OK) {
        spaceship_draw(ship);
    } else if (spaceship_state == SPACESHIP_STATE_DESTROYED) {
        int i;

        for (i = 0; i < SPACESHIP_NUM_VERTICES; i++) {
            object_draw(spaceship_parts[i]);
        }
    }

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

        if (key == SDLK_r) {
            spaceship_state = SPACESHIP_STATE_OK;
        }
    }
}

void handle_keys()
{
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    // rotate spaceship
    if (keystates[SDL_SCANCODE_LEFT]) {
        ship->angular_speed = SPACESHIP_ANGULAR_VELOCITY * -1;
    } else if (keystates[SDL_SCANCODE_RIGHT]) {
        ship->angular_speed = SPACESHIP_ANGULAR_VELOCITY;
    } else {
        ship->angular_speed = 0;
    }

    if (keystates[SDL_SCANCODE_UP]) {
        update_ship_velocity(SPACESHIP_MAX_SPEED, SPACESHIP_ACCELERATION);
    } else {
        update_ship_velocity(0, SPACESHIP_DECELERATION);
    }
}

/**
 * Adjust the ship's x and y velocity components, so it eventually reaches a maximum velocity in the
 * direction it's pointing to.
 */
void update_ship_velocity(float target_speed, float acceleration)
{
    Point max_vel;

    // calculate maximum velocity in the current direction
    vector_get_components(ship->direction, target_speed, &max_vel);

    // update spaceship velocity
    ship->velocity.x += acceleration * time_step * (max_vel.x - ship->velocity.x);
    ship->velocity.y += acceleration * time_step * (max_vel.y - ship->velocity.y);
}

void shoot()
{
    Bullet *bullet;

    bullet = bullet_new(
        ship->points[SPACESHIP_TOP_VERTEX].x,
        ship->points[SPACESHIP_TOP_VERTEX].y,
        ship->direction,
        BULLET_SPEED);

    list_append(&bullets, (void *)bullet);
}

void update_bullets()
{
    Node **node_ref;
    Bullet *bullet;
    Point trail;

    node_ref = &bullets;

    while (*node_ref != NULL) {
        bullet = (Bullet *)(*node_ref)->data;
        trail = bullet->points[1];
        bullet_update(bullet);

        if (check_bullet_collision(bullet, &trail)) {
            bullet_delete(bullet);
            *node_ref = destroy_node(*node_ref);
        } else {
            node_ref = &(*node_ref)->next;
        }
    }
}

SDL_bool check_bullet_collision(Bullet *bullet, Point *trail)
{
    Node **node_ref = &asteroids;
    Asteroid *asteroid;

    // check edge collision
    if (object_is_off_screen((Object *)bullet)) {
        return SDL_TRUE;
    }

    // check asteroid collision
    while (*node_ref != NULL) {
        asteroid = (Asteroid *)(*node_ref)->data;

        if (object_intersect_line((Object *)asteroid, *trail, bullet->points[0])) {
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
        bullet_draw(node->data);
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
        asteroid_update(asteroid);
        node_ref = &(*node_ref)->next;
    }
}

void draw_asteroids(SDL_Renderer *renderer)
{
    Node *node = asteroids;

    while (node != NULL) {
        asteroid_draw(node->data);
        node = node->next;
    }
}
