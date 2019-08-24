#include <stdio.h>
#include "game.h"
#include "spaceship.h"
#include "asteroid.h"
#include "bullet.h"
#include "list.h"
#include "timer.h"

static void enter();
static void update();
static void render();
static void handle_event(SDL_Event *event);
void load_media();
void handle_keys();
void next_level();
void add_asteroid();
void handle_collisions();
void shoot();
SDL_bool bullet_handle_collision(Bullet *bullet, Point *tail);
void asteroid_destroy(Asteroid *asteroid);

Scene game = {enter, update, render, handle_event};

SDL_Renderer *renderer;
Mix_Chunk *sounds[SFX_COUNT];

Node *asteroids;
Node *bullets;
Spaceship *ship;

float time_step;
int lives;
int level;
int points;

void enter()
{
    int i;

    load_media();

    // Reserve channel for trust sound
    Mix_PlayChannel(SFX_THRUST_CHANNEL, sounds[SFX_THRUST], -1);
    Mix_Pause(SFX_THRUST_CHANNEL);

    points = 0;
    level = 0;
    lives = GAME_INITIAL_LIVES;

    // Create the spaceship
    ship = spaceship_new(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, -PI/2, 0);

    next_level();
}

void update()
{
    spaceship_update(ship);

    if (ship->state & SPACESHIP_STATE_DESTROYED) {
        // When destroyed, the spaceship is restored after a given time (respawning).
        if (lives >= 0 &&
            timer_get_seconds(&ship->state_timer) >= GAME_RESPAWNING_TIME) {

            // Relocate spaceship
            object_reset((Object *)ship,
                         WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,
                         -PI/2, 0);

            // After respawning, the spaceship is set into invincible mode for a
            // brief moment. The timer is restarted so we know how much time the
            // spaceship has been in this state.
            ship->state = SPACESHIP_STATE_INVINCIBLE | SPACESHIP_STATE_OK;
            timer_start(&ship->state_timer);
        }
    } else {
        // The spaceship invincivility should be removed after the given amount
        // of time.
        if (ship->state & SPACESHIP_STATE_INVINCIBLE &&
            timer_get_seconds(&ship->state_timer) >= GAME_INVINCIBILITY_TIME) {

            ship->state ^= SPACESHIP_STATE_INVINCIBLE;
        }
    }

    objects_update(asteroids);

    handle_collisions();
    handle_keys();
}

void render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    spaceship_draw(ship);

    objects_draw(bullets);
    objects_draw(asteroids);
}

void handle_event(SDL_Event *event)
{
    SDL_Keycode key;

    // Handle the keydown event
    if (event->type == SDL_KEYDOWN && ! event->key.repeat) {
        key = event->key.keysym.sym;

        // Handle Spaceship controls
        if (ship->state & SPACESHIP_STATE_OK) {
            if (key == SDLK_SPACE) {
                shoot();
            }
        }
    }
}

void load_media() {
    // Load sounds
    sounds[SFX_FIRE] = Mix_LoadWAV("media/sound/fire.wav");
    sounds[SFX_EXPLOSION_LARGE] = Mix_LoadWAV("media/sound/bangLarge.wav");
    sounds[SFX_EXPLOSION_MEDIUM] = Mix_LoadWAV("media/sound/bangMedium.wav");
    sounds[SFX_EXPLOSION_SMALL] = Mix_LoadWAV("media/sound/bangSmall.wav");
    sounds[SFX_THRUST] = Mix_LoadWAV("media/sound/thrust.wav");
}

void handle_keys()
{
    const Uint8* keystates = SDL_GetKeyboardState(NULL);

    // Get keyboard state and apply controls to the spaceship: LEFT and RIGHT
    // keys adjust the spaceship direction, while UP key adjust the spaceship
    // velocity. This is not done through the SDL_KEYDOWN event, since we need
    // to continuously apply these controls while the keys are pressed.

    if (ship->state & SPACESHIP_STATE_DESTROYED) {
        return;
    }

    if (keystates[SDL_SCANCODE_LEFT]) {
        ship->obj.angular_speed = SPACESHIP_ANGULAR_VELOCITY * -1;
    } else if (keystates[SDL_SCANCODE_RIGHT]) {
        ship->obj.angular_speed = SPACESHIP_ANGULAR_VELOCITY;
    } else {
        ship->obj.angular_speed = 0;
    }

    if (keystates[SDL_SCANCODE_UP]) {
        spaceship_update_velocity(ship, SPACESHIP_MAX_SPEED, SPACESHIP_ACCELERATION);

        if (! (ship->state & SPACESHIP_STATE_THRUST)) {
            ship->state |= SPACESHIP_STATE_THRUST;
            Mix_Resume(SFX_THRUST_CHANNEL);
        }
    } else {
        spaceship_update_velocity(ship, 0, SPACESHIP_DECELERATION);

        if (ship->state & SPACESHIP_STATE_THRUST) {
            ship->state ^= SPACESHIP_STATE_THRUST;
            Mix_Pause(SFX_THRUST_CHANNEL);
        }
    }
}

void next_level()
{
    int i, count;

    // Set up the asteroids for the next level
    count = LEVEL_INITIAL_ASTEROIDS + (level * LEVEL_ASTEROID_INCREMENT);

    for (i = 0; i < count; i++) {
        add_asteroid();
    }

    level++;
}

void add_asteroid()
{
    Asteroid *asteroid;
    int r, x, y;

    // Asteroids are placed on the screen borders: top, bottom, left, right
    r = rand() % 4;

    if (r < 2) {
        // top or bottom border
        x = uniform(0, WINDOW_WIDTH);
        y = (r == 0) ? 0 : WINDOW_HEIGHT;
    } else {
        // left or right border
        x = (r == 2) ? 0 : WINDOW_WIDTH;
        y = uniform(0, WINDOW_HEIGHT);
    }

    asteroid = asteroid_new(
            x, y,
            ASTEROID_LARGE,
            uniform(0, 2 * PI),
            ASTEROID_SPEED,
            randint(ASTEROID_MIN_SIDES, ASTEROID_MAX_SIDES));

    list_append(&asteroids, (void *)asteroid);
}

void handle_collisions()
{
    Node *node;

    // Check collision between spaceship and asteroids.
    if (ship->state & SPACESHIP_STATE_OK &&
        ! (ship->state & SPACESHIP_STATE_INVINCIBLE)) {
        node = asteroids;

        while (node != NULL) {
            if (object_check_collision((Object *)ship, (Object *)node->data)) {
                spaceship_destroy(ship);
                break;
            }

            node = node->next;
        }
    }

    // Check collision between bullets and asteroids.
    Node **node_ref;
    Bullet *bullet;
    Point tail;

    node_ref = &bullets;

    while (*node_ref != NULL) {
        bullet = (Bullet *)(*node_ref)->data;

        // Save the bullet trail before updating its position. The collision is
        // determined against the bullet trajectory and not the bullet itself,
        // this is, the bullet tail from last position to the current bullet
        // head; otherwise the bullet could pass through an object without
        // colliding.
        tail = bullet->points[BULLET_TAIL];

        bullet_update(bullet);

        if (bullet_handle_collision(bullet, &tail)) {
            bullet_delete(bullet);
            *node_ref = destroy_node(*node_ref);
        } else {
            node_ref = &(*node_ref)->next;
        }
    }
}

void shoot()
{
    Bullet *bullet;

    bullet = bullet_new(
        ship->obj.points[SPACESHIP_TOP_VERTEX].x,
        ship->obj.points[SPACESHIP_TOP_VERTEX].y,
        ship->obj.direction,
        BULLET_SPEED);

    list_append(&bullets, (void *)bullet);

    Mix_PlayChannel(-1, sounds[SFX_FIRE], 0);
}

SDL_bool bullet_handle_collision(Bullet *bullet, Point *tail)
{
    Node **node_ref;
    Asteroid *asteroid;

    // Check screen edge collision
    if (object_is_off_screen((Object *)bullet)) {
        return SDL_TRUE;
    }

    // Check collision with asteroids
    node_ref = &asteroids;

    while (*node_ref != NULL) {
        asteroid = (Asteroid *)(*node_ref)->data;

        if (object_intersect_line(
                (Object *)asteroid,
                *tail,
                bullet->points[BULLET_HEAD])) {

            asteroid_destroy(asteroid);
            asteroid_delete(asteroid);

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
    Asteroid *fragment;
    int i, radius, effect, fragments;

    // When an asteroid is destroyed, we create smaller asteroids (fragments)
    // from it. These smaller asteroids move faster and point in a similar
    // direction.

    fragments = ASTEROID_FRAGMENTS;

    if (asteroid->radius == ASTEROID_LARGE) {
        effect = SFX_EXPLOSION_LARGE;
        radius = ASTEROID_MEDIUM;
    } else if (asteroid->radius == ASTEROID_MEDIUM) {
        effect = SFX_EXPLOSION_MEDIUM;
        radius = ASTEROID_SMALL;
    } else {
        effect = SFX_EXPLOSION_SMALL;
        fragments = 0;
    }

    for (i = 0; i < fragments; i++) {
        fragment = asteroid_new(
                asteroid->position.x, asteroid->position.y,
                radius,
                asteroid->direction + uniform(0.8, -0.8),
                (ASTEROID_SPEED - radius) * 3,
                uniform(ASTEROID_MIN_SIDES, ASTEROID_MAX_SIDES));

        list_append(&asteroids, (void *)fragment);
    }

    Mix_PlayChannel(-1, sounds[effect], 0);
}
