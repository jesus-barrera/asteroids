#include <math.h>
#include <stdlib.h>
#include "game.h"
#include "object.h"

static void wrap(float *pos, int min, int max);

Object *object_new(int x, int y, float radius, float direction, float speed, int num_points)
{
    Object *obj = (Object *)malloc(sizeof(Object));

    obj->position.x = x;
    obj->position.y = y;
    obj->radius = radius;
    obj->direction = direction;
    obj->num_points = num_points;
    obj->points = (Point *)malloc(sizeof(Point) * num_points);

    set_velocity(&obj->velocity, direction, speed);

    return obj;
}

void object_delete(Object *obj)
{
    free(obj->points);
    free(obj);
}

void object_move(Object *obj)
{
    int i;
    Point offset;
    Point position;

    position = obj->position;

    object_update_position(obj);

    wrap(&(obj->position.x), 0, game_viewport.w);
    wrap(&(obj->position.y), 0, game_viewport.h);

    // calculate object displecement from last position
    offset.x = obj->position.x - position.x;
    offset.y = obj->position.y - position.y;

    // apply displacement to the object's points
    for (i = 0; i < obj->num_points; i++) {
        obj->points[i].x += offset.x;
        obj->points[i].y += offset.y;
    }
}

void object_update_position(Object *obj)
{
    obj->position.x += obj->velocity.x * time_step;
    obj->position.y += obj->velocity.y * time_step;
}

void object_rotate(Object *obj, float radians)
{
    float x, y;
    int i;

    // calculate new points positions
    for (i = 0; i < obj->num_points; i++) {
        x = obj->points[i].x - obj->position.x;
        y = obj->points[i].y - obj->position.y;

        obj->points[i].x = x * cos(radians) - y * sin(radians) + obj->position.x;
        obj->points[i].y = y * cos(radians) + x * sin(radians) + obj->position.y;
    }

    obj->direction += radians;
}

SDL_bool object_check_collision(Object *a, Object *b)
{
    // calculate the euclidean distance between the objects
    float distance = sqrt(
                        pow(b->position.x - a->position.x, 2) +
                        pow(b->position.y - a->position.y, 2));

    // test if the object's circumscribed circles overlap, if not, then they aren't
    // close enough to collide.
    if (distance > (a->radius + b->radius)) {
        return SDL_FALSE;
    }

    // check collision by testing if any edge of one object intersects any edge
    // of the other object.

    int i, j, count;
    Point p1, p2;

    // handle single line objects
    count = (a->num_points == 2) ? 1 : a->num_points;

    for (i = 0; i < count; i++) {
        // get next point index
        j = (i + 1) % a->num_points;

        p1 = a->points[i];
        p2 = a->points[j];

        if (object_intersect_line(b, p1, p2)) {
            return SDL_TRUE;
        }
    }

    return SDL_FALSE;
}

SDL_bool object_intersect_line(Object *obj, Point p1, Point p2)
{
    int i, j, count;
    Point q1, q2;

    count = (obj->num_points == 2) ? 1 : obj->num_points;

    for (i = 0; i < count; i++) {
        j = (i + 1) % obj->num_points;

        q1 = obj->points[i];
        q2 = obj->points[j];

        if (lines_intersect(p1, p2, q1, q2)) {
            return SDL_TRUE;
        }
    }

    return SDL_FALSE;
}

void object_draw(Object *obj)
{
    int i, j, count;
    Point q1, q2;

    count = (obj->num_points == 2) ? 1 : obj->num_points;

    for (i = 0; i < count; i++) {
        j = (i + 1) % obj->num_points;

        SDL_RenderDrawLine(
            renderer,
            obj->points[i].x, obj->points[i].y,
            obj->points[j].x, obj->points[j].y);
    }
}

void object_set_point(Object *obj, int point, float angle)
{
    angle += obj->direction;

    obj->points[point].x = obj->position.x + cos(angle) * obj->radius;
    obj->points[point].y = obj->position.y + sin(angle) * obj->radius;
}

SDL_bool object_is_off_screen(Object *obj)
{
    return (obj->position.x < 0 || obj->position.x > game_viewport.w ||
            obj->position.y < 0 || obj->position.y > game_viewport.h);
}

void set_velocity(Point *velocity, float direction, float speed)
{
    velocity->x = cos(direction) * speed;
    velocity->y = sin(direction) * speed;
}

void wrap(float *pos, int min, int max)
{
    if (*pos > max) {
        *pos = min;
    } else if (*pos < min) {
        *pos = max;
    }
}
