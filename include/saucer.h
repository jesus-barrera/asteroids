#ifndef _SAUCER_H_INCLUDED_
#define _SAUCER_H_INCLUDED_

#include "object.h"

#define SAUCER_VERTICES_COUNT 8

#define saucer_update(saucer) object_update((Object *)saucer)
#define saucer_delete(saucer) object_delete((Object *)saucer)

typedef struct Object Saucer;

Saucer *saucer_new(int x, int y, float radius, float direction, float speed);
void saucer_draw(Saucer *saucer);

#endif
