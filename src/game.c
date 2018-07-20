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
void asteroid_destroy(Asteroid *asteroid);
void add_asteroid();
void draw_bullets(SDL_Renderer *renderer);
void update_asteroids();
void draw_asteroids(SDL_Renderer *renderer);

enum {
    SPACESHIP_STATE_OK,
    SPACESHIP_STATE_DESTROYED
};

Node *asteroids;
Node *bullets;
Node *parts;
Spaceship *ship;

SDL_Renderer *renderer;
Timer respawn_timer;
float time_step;
int state;
int lives;
int level;

void init_game()
{
    int i;

    level = 0;
    lives = PLAYER_INITIAL_LIVES;

    // create the spaceship
    ship = spaceship_new(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, -PI/2, 0);

    // spaceship parts
    for (i = 0; i < ship->num_points; i++) {
        list_append(&parts, (void *)object_create(0, 0, 0, 0, 2));
    }

    next_level();
}

void add_asteroid()
{
    Asteroid *asteroid;
    int r, x, y;

    // asteroids are placed on the screen borders: up, down, left, right
    r = rand() % 4;

    if (r < 2) {
        x = uniform(0, WINDOW_WIDTH);
        y = (r == 0) ? 0 : WINDOW_HEIGHT;
    } else {
        x = (r == 2) ? 0 : WINDOW_WIDTH;
        y = uniform(0, WINDOW_HEIGHT);
    }

    asteroid = asteroid_new(
            x, y,
            ASTEROID_LARGE,
            uniform(0, 2 * PI),
            uniform(ASTEROID_MIN_SPEED, ASTEROID_MAX_SPEED),
            randint(ASTEROID_MIN_SIDES, ASTEROID_MAX_SIDES));

    list_append(&asteroids, (void *)asteroid);
}

void next_level()
{
    int i, count;

    count = LEVEL_INITIAL_ASTEROIDS + (level * LEVEL_ASTEROID_INCREMENT);

    for (i = 0; i < count; i++) {
        add_asteroid();
    }

    level++;
}

void update()
{
    handle_keys();

    if (spaceship_state == SPACESHIP_STATE_OK) {
        spaceship_update(ship);
    } else if (spaceship_state == SPACESHIP_STATE_DESTROYED) {
        objects_update(spaceship_parts);

        if (lives > 0 && timer_get_seconds(&respawn_timer) >= SPACESHIP_RESPAWN_TIME) {
            object_reset((Object *)ship, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, -PI/2, 0);

            spaceship_state = SPACESHIP_STATE_OK;
        }
    }

    objects_update(asteroids);

    // handle collisions
    handle_collisions();
}

void render(SDL_Renderer *renderer)
{
    SDL_RenderSetViewport(renderer, &game_viewport);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    if (spaceship_state == SPACESHIP_STATE_OK) {
        spaceship_draw(ship);
    } else if (spaceship_state == SPACESHIP_STATE_DESTROYED) {
        objects_draw(parts);
    }

    objects_draw(bullets);
    objects_draw(asteroids);
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

void handle_collisions()
{
    Node *node;

    // spaceship collision with asteroid
    node = asteroids;

    while (node != NULL) {
        if (object_check_collision((Object *)ship, (Object *)node->data)) {
            spaceship_destroy();
            break;
        }

        node = node->next;
    }

    // bullet collision
    Bullet *bullet;
    Point trail;

    node = bullets;

    while (*node != NULL) {
        bullet = (Bullet *)(node)->data;
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

void spaceship_destroy()
{
    int i, j;
    Point p1, p2;
    Node *node;
    Object *part;

    for (i = 0, node = parts; i < ship->num_points; i++, node = node->next) {
        j = (i + 1) % ship->num_points;
        part = (Object *)node->data;

        // form line from point i to j
        p1 = ship->points[i];
        p2 = ship->points[j];

        // set line center
        part->position.x = p1.x + (p2.x - p1.x) / 2;
        part->position.y = p1.y + (p2.y - p1.y) / 2;
        part->direction = uniform(0, PI * 2);
        vector_set_components(part->direction, 20, &part->velocity);
        part->angular_speed = uniform(-PI, PI);
        part->points[0] = p1;
        part->points[1] = p2;
    }

    // update state
    spaceship_state = SPACESHIP_STATE_DESTROYED;

    // start timer for respawining
    timer_start(&respawn_timer);
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
    vector_set_components(ship->direction, target_speed, &max_vel);

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
            // destroy asteroid
            asteroid_destroy(asteroid);
            asteroid_delete(asteroid);

            // remove from list
            *node_ref = destroy_node(*node_ref);

            return SDL_TRUE;
        } else {
            node_ref = &(*node_ref)->next;
        }
    }

    return SDL_FALSE;
}

void asteroid_destroy(Asteroid *asteroid)
{
    int i, radius;

    if (asteroid->radius == ASTEROID_RADIUS_LARGE) {
        radius = ASTEROID_RADIUS_MEDIUM;
    } else if (asteroid->radius == ASTEROID_RADIUS_MEDIUM) {
        radius = ASTEROID_RADIUS_SMALL;
    } else {
        return;
    }

    for (i = 0; i < ASTEROID_FRAGMENTS; i++) {
        asteroid = asteroid_new(
                asteroid->position.x, asteroid->position.y,
                radius,
                asteroid->direction + uniform(-1, 1),
                (ASTEROID_SPEED - radius) * 2,
                ASTEROID_SIDES);

        list_append(&asteroids, (void *)asteroid);
    }
}

void update_objects_list(Node *node)
{
    while (node != NULL) {
        object_update((Object *)node->data);
        node = node->next;
    }
}

void update_objects_arry(Object objects[], int count)
{

}

void draw_objects(Node *node)
{
    while (node != NULL) {
        object_draw((Object *)node->data);
        node = node->next;
    }
}
