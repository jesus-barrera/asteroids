#include <stdlib.h>
#include "tools.h"

float uniform(float min, float max)
{
    return (rand() / (float)RAND_MAX) * (max - min) + min;
}
