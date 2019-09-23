#include <math.h>
#include <stdlib.h>
#include "game.h"
#include "geometry.h"
#include "object.h"

static void wrap(float *pos, int min, int max);

Object *object_new(int x, int y, int radius, float direction, float speed, int num_points)
{
    Object *obj = (Object *)malloc(sizeof(Object));

    object_set_props(obj, x, y, radius, direction, speed, num_points);

    return obj;
}

void object_set_props(Object *obj, int x, int y, int radius, float direction, float speed, int num_points)
{
    obj->position.x = x;
    obj->position.y = y;
    obj->radius = radius;
    obj->direction = direction;
    obj->angular_speed = 0;
    obj->wrap = SDL_TRUE;
    obj->num_points = num_points;
    obj->points = (Point *)malloc(sizeof(Point) * num_points);
    obj->outline_end = num_points;

    vector_set_components(direction, speed, &obj->velocity);
}

void object_delete(Object *obj)
{
    free(obj->points);
    free(obj);
}

void object_update(Object *obj)
{
    Point displacement;
    float rotation;

    // calculate rotation
    rotation = obj->angular_speed * time_step;

    // calculate position displacement
    vector_mult_by_scalar(&obj->velocity, time_step, &displacement);

    if (obj->wrap) {
        Point new_position;

        vector_add(&obj->position, &displacement, &new_position);

        wrap(&new_position.x, 0, WINDOW_WIDTH);
        wrap(&new_position.y, 0, WINDOW_HEIGHT);

        vector_diff(&new_position, &obj->position, &displacement);
    }

    // update object
    object_move(obj, &displacement);
    object_rotate(obj, rotation);
}

void object_move(Object *obj, Point *displacement)
{
    int i;

    vector_add(&obj->position, displacement, &obj->position);

    for (i = 0; i < obj->num_points; i++) {
        vector_add(&obj->points[i], displacement, &obj->points[i]);
    }
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
    if (! object_circle_collision(a, b)) {
        return SDL_FALSE;
    }

    // check collision by testing if any edge of one object intersects any edge
    // of the other object.

    int i, j, count;
    Point p1, p2;

    // handle single line objects
    count = (a->outline_end == 2) ? 1 : a->outline_end;

    for (i = 0; i < count; i++) {
        // get next point index
        j = (i + 1) % a->outline_end;

        p1 = a->points[i];
        p2 = a->points[j];

        if (object_intersect_line(b, p1, p2)) {
            return SDL_TRUE;
        }
    }

    return SDL_FALSE;
}

SDL_bool object_circle_collision(Object *a, Object *b)
{
    // Calculate the euclidean distance between the objects
    float distance = sqrt(
                        pow(b->position.x - a->position.x, 2) +
                        pow(b->position.y - a->position.y, 2));

    // Test if the object's circumscribed circles overlap, if not, then they aren't
    // close enough to collide.
    if (distance > (a->radius + b->radius)) {
        return SDL_FALSE;
    }
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

    count = (obj->outline_end == 2) ? 1 : obj->outline_end;

    for (i = 0; i < count; i++) {
        j = (i + 1) % obj->outline_end;

        SDL_RenderDrawLine(
            renderer,
            obj->points[i].x, obj->points[i].y,
            obj->points[j].x, obj->points[j].y);
    }
}

void object_draw_lines(Object *obj, int *points)
{
    int i;
    int p1, p2;

    for (i = 0; points[i] != -1; i += 2) {
        p1 = points[i];
        p2 = points[i + 1];

        SDL_RenderDrawLine(
            renderer,
            obj->points[p1].x, obj->points[p1].y,
            obj->points[p2].x, obj->points[p2].y);
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
    return (obj->position.x < 0 || obj->position.x > WINDOW_WIDTH ||
            obj->position.y < 0 || obj->position.y > WINDOW_HEIGHT);
}

void object_reset(Object *obj, int x, int y, float direction, float speed)
{
    Point displacement;
    float rotation;

    // calculate object displacement from current position.
    displacement.x = x - obj->position.x;
    displacement.y = y - obj->position.y;

    // calculate object rotation
    rotation = direction - obj->direction;

    // apply displacement and rotation
    object_move(obj, &displacement);
    object_rotate(obj, rotation);

    // set velocity
    vector_set_components(obj->direction, speed, &obj->velocity);
};

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

void wrap(float *pos, int min, int max)
{
    if (*pos > max) {
        *pos = min;
    } else if (*pos < min) {
        *pos = max;
    }
}

void vector_set_components(float direction, float magnitude, Point *result)
{
    result->x = cos(direction) * magnitude;
    result->y = sin(direction) * magnitude;
}

void vector_add(Point *v, Point *u, Point *result)
{
    result->x = v->x + u->x;
    result->y = v->y + u->y;
}

void vector_diff(Point *v, Point *u, Point *result)
{
    result->x = v->x - u->x;
    result->y = v->y - u->y;
}

void vector_mult_by_scalar(Point *v, float scalar, Point *result)
{
    result->x = v->x * scalar;
    result->y = v->y * scalar;
}
