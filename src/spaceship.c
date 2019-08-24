#include <stdlib.h>
#include <math.h>
#include "spaceship.h"
#include "game.h"

/*
 * Spaceship points coordinates given as the distance and angle from its center.
 */
static float points[][SPACESHIP_TOTAL_POINTS_COUNT] = {
    // Spaceship's points
    {1.00, 0.00},
    {1.00, 2.50},
    {0.60, 2.65},
    {0.60, -2.65},
    {1.00, -2.50},

    // Flame's points
    {0.80, 2.85},
    {1.00, -PI},
    {0.80, -2.85},
};

static int flame_lines[] = {
    2, 5,
    5, 6,
    6, 7,
    7, 3,
    -1
};

static void spaceship_set_points(Spaceship *ship);

Spaceship *spaceship_new(int x, int y, float direction, float speed)
{
    int i;

    Spaceship *ship = (Spaceship *)malloc(sizeof(Spaceship));

    object_set_props(
        &ship->obj,
        x, y,
        SPACESHIP_RADIUS, direction,
        speed, SPACESHIP_TOTAL_POINTS_COUNT);

    ship->obj.outline_end = SPACESHIP_POINTS_COUNT;
    ship->state = SPACESHIP_STATE_OK;

    spaceship_set_points(ship);

    // Create spaceship "parts". These are used to do the "explode effect" when
    // the spaceship is destroyed.
    for (i = 0; i < SPACESHIP_POINTS_COUNT; i++) {
        list_append(&ship->parts, (void *)object_new(0, 0, 0, 0, 0, 2));
    }

    return ship;
}

void spaceship_delete(Spaceship *ship)
{

}

void spaceship_update(Spaceship *ship)
{
    if (ship->state & SPACESHIP_STATE_OK) {
        object_update((Object *) ship);

        if (ship->state & SPACESHIP_STATE_THRUST) {
            if (timer_get_seconds(&ship->flickering_timer) >=
                SPACESHIP_FLAME_FLICKERING_TIME) {

                ship->draw_flame = ! ship->draw_flame;
                timer_start(&ship->flickering_timer);
            }
        } else {
            ship->draw_flame = SDL_FALSE;
        }
    } else {
        objects_update(ship->parts);
    }
}

void spaceship_draw(Spaceship *ship)
{
    if (ship->state & SPACESHIP_STATE_OK) {
        object_draw((Object *)ship);

        if (ship->draw_flame) {
            object_draw_lines((Object *)ship, flame_lines);
        }
    } else {
        objects_draw(ship->parts);
    }
}

void spaceship_destroy(Spaceship *ship)
{
    int i, j;
    Point p1, p2;
    Node *node;
    Object *part;

    // For each side of the spaceship we create a new object (part) that moves
    // independently, this makes the effect of the spaceship being destroyed.
    for (i = 0, node = ship->parts; i < SPACESHIP_POINTS_COUNT; i++, node = node->next) {
        j = (i + 1) % SPACESHIP_POINTS_COUNT;
        part = (Object *)node->data;

        // Create a line from point i to j.
        p1 = part->points[0] = ship->obj.points[i];
        p2 = part->points[1] = ship->obj.points[j];

        // Set the center at the middle of the line.
        part->position.x = p1.x + (p2.x - p1.x) / 2;
        part->position.y = p1.y + (p2.y - p1.y) / 2;

        part->direction = uniform(0, PI * 2);

        vector_set_components(part->direction, 20, &part->velocity);

        part->angular_speed = uniform(-2 * PI, 2 * PI);
    }

    Mix_Pause(SFX_THRUST_CHANNEL);
    Mix_PlayChannel(-1, sounds[SFX_EXPLOSION_MEDIUM], 0);

    ship->state = SPACESHIP_STATE_DESTROYED;

    timer_start(&ship->state_timer);
}


void spaceship_update_velocity(Spaceship *ship, float target_speed, float acceleration)
{
    Point vel;

    // Calculate target velocity in the current direction
    vector_set_components(ship->obj.direction, target_speed, &vel);

    // Update spaceship velocity
    ship->obj.velocity.x += acceleration * time_step * (vel.x - ship->obj.velocity.x);
    ship->obj.velocity.y += acceleration * time_step * (vel.y - ship->obj.velocity.y);
}

void spaceship_set_points(Spaceship *ship)
{
    int i, j;

    for (i = 0, j = 8; i < j; i++) {
        // Convert to Cartesian coordinates.
        vector_set_components(
            points[i][1] + ship->obj.direction,
            points[i][0] * ship->obj.radius,
            &ship->obj.points[i]);

        // Apply displacement.
        vector_add(&ship->obj.points[i], &ship->obj.position, &ship->obj.points[i]);
    }
}
