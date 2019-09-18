#include <stdlib.h>
#include "game.h"
#include "tools.h"

float uniform(float min, float max)
{
    return (rand() / (float)RAND_MAX) * (max - min) + min;
}

int randint(int min, int max)
{
    return rand() % (max - min) + min;
}
