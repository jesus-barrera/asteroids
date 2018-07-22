#include <stdio.h>
#include <SDL_mixer.h>
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
void spaceship_update_velocity(float target_speed, float acceleration);
void spaceship_destroy();
SDL_bool bullet_handle_collision(Bullet *bullet, Point *tail);
void asteroid_destroy(Asteroid *asteroid);
void objects_update(Node *node);
void objects_draw(Node *node);

SDL_Renderer *renderer;

// Sound effects
enum {
    SFX_FIRE,
    SFX_EXPLOSION_LARGE,
    SFX_EXPLOSION_MEDIUM,
    SFX_EXPLOSION_SMALL,
    SFX_THRUST,
    SFX_COUNT
};

#define SFX_THRUST_CHANNEL 0

Mix_Chunk *sounds[SFX_COUNT];

Scene game = {enter, update, render, handle_event};

Node *asteroids;
Node *bullets;
Node *parts;
Spaceship *ship;

Timer spaceship_timer;
float time_step;
int spaceship_state;
int lives;
int level;

void enter()
{
    int i;

    load_media();

    // Reserve channel for trust sound
    Mix_PlayChannel(SFX_THRUST_CHANNEL, sounds[SFX_THRUST], -1);
    Mix_Pause(SFX_THRUST_CHANNEL);

    level = 0;
    lives = GAME_INITIAL_LIVES;

    // Create the spaceship
    ship = spaceship_new(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, -PI/2, 0);

    // Create spaceship "parts". These are used to do the "explode effect" when
    // the spaceship is destroyed.
    for (i = 0; i < ship->num_points; i++) {
        list_append(&parts, (void *)object_new(0, 0, 0, 0, 0, 2));
    }

    next_level();
}

void update()
{
    if (spaceship_state == SPACESHIP_STATE_DESTROYED) {
        objects_update(parts);

        // When destroyed, the spaceship is restored after a given time (respawning).
        if (lives >= 0 &&
            timer_get_seconds(&spaceship_timer) >= GAME_RESPAWNING_TIME) {

            // Relocate spaceship
            object_reset((Object *)ship,
                         WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2,
                         -PI/2, 0);

            // After respawning, the spaceship is set into invincible mode for a
            // brief moment. The timer is restarted so we know how much time the
            // spaceship has been in this state.
            spaceship_state = SPACESHIP_STATE_INVINCIBLE;
            timer_start(&spaceship_timer);
        }
    } else {
        spaceship_update(ship);

        // The spaceship invincivility should be removed after the given amount
        // of time.
        if (spaceship_state == SPACESHIP_STATE_INVINCIBLE &&
            timer_get_seconds(&spaceship_timer) >= GAME_INVINCIBILITY_TIME) {

            spaceship_state = SPACESHIP_STATE_OK;
        }
    }

    objects_update(asteroids);

    handle_collisions();
    handle_keys();
}

void render(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    if (spaceship_state == SPACESHIP_STATE_DESTROYED) {
        objects_draw(parts);
    } else {
        spaceship_draw(ship);
    }

    objects_draw(bullets);
    objects_draw(asteroids);
}

void handle_event(SDL_Event *event)
{
    SDL_Keycode key;

    // Handle the keydown event
    if (event->type == SDL_KEYDOWN && ! event->key.repeat) {
        key = event->key.keysym.sym;

        if (key == SDLK_SPACE && spaceship_state != SPACESHIP_STATE_DESTROYED) {
            shoot();
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

    if (spaceship_state == SPACESHIP_STATE_DESTROYED) {
        return;
    }

    if (keystates[SDL_SCANCODE_LEFT]) {
        ship->angular_speed = SPACESHIP_ANGULAR_VELOCITY * -1;
    } else if (keystates[SDL_SCANCODE_RIGHT]) {
        ship->angular_speed = SPACESHIP_ANGULAR_VELOCITY;
    } else {
        ship->angular_speed = 0;
    }

    if (keystates[SDL_SCANCODE_UP]) {
        spaceship_update_velocity(SPACESHIP_MAX_SPEED, SPACESHIP_ACCELERATION);
        Mix_Resume(SFX_THRUST_CHANNEL);
    } else {
        spaceship_update_velocity(0, SPACESHIP_DECELERATION);
        Mix_Pause(SFX_THRUST_CHANNEL);
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
    if (spaceship_state == SPACESHIP_STATE_OK) {
        node = asteroids;

        while (node != NULL) {
            if (object_check_collision((Object *)ship, (Object *)node->data)) {
                spaceship_destroy();
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
        ship->points[SPACESHIP_TOP_VERTEX].x,
        ship->points[SPACESHIP_TOP_VERTEX].y,
        ship->direction,
        BULLET_SPEED);

    list_append(&bullets, (void *)bullet);

    Mix_PlayChannel(-1, sounds[SFX_FIRE], 0);
}

void spaceship_update_velocity(float target_speed, float acceleration)
{
    Point vel;

    // Calculate target velocity in the current direction
    vector_set_components(ship->direction, target_speed, &vel);

    // Update spaceship velocity
    ship->velocity.x += acceleration * time_step * (vel.x - ship->velocity.x);
    ship->velocity.y += acceleration * time_step * (vel.y - ship->velocity.y);
}

void spaceship_destroy()
{
    int i, j;
    Point p1, p2;
    Node *node;
    Object *part;

    // For each side of the spaceship we create a new object (part) that moves
    // independently, this makes the effect of the spaceship being destroyed.
    for (i = 0, node = parts; i < ship->num_points; i++, node = node->next) {
        j = (i + 1) % ship->num_points;
        part = (Object *)node->data;

        // Create a line from point i to j.
        p1 = part->points[0] = ship->points[i];
        p2 = part->points[1] = ship->points[j];

        // Set the center at the middle of the line.
        part->position.x = p1.x + (p2.x - p1.x) / 2;
        part->position.y = p1.y + (p2.y - p1.y) / 2;

        part->direction = uniform(0, PI * 2);

        vector_set_components(part->direction, 20, &part->velocity);

        part->angular_speed = uniform(-2 * PI, 2 * PI);
    }

    Mix_Pause(SFX_THRUST_CHANNEL);
    Mix_PlayChannel(-1, sounds[SFX_EXPLOSION_MEDIUM], 0);

    lives -= 1;

    spaceship_state = SPACESHIP_STATE_DESTROYED;

    // Start respawning timer.
    timer_start(&spaceship_timer);
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
        asteroid = asteroid_new(
                asteroid->position.x, asteroid->position.y,
                radius,
                asteroid->direction + uniform(-PI/2, PI/2),
                (ASTEROID_SPEED - radius) * 3,
                uniform(ASTEROID_MIN_SIDES, ASTEROID_MAX_SIDES));

        list_append(&asteroids, (void *)asteroid);
    }

    Mix_PlayChannel(-1, sounds[effect], 0);
}

void objects_update(Node *node)
{
    while (node != NULL) {
        object_update((Object *)node->data);
        node = node->next;
    }
}

void objects_draw(Node *node)
{
    while (node != NULL) {
        object_draw((Object *)node->data);
        node = node->next;
    }
}
