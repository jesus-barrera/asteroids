#include "timer.h"

void timer_start(Timer *timer)
{
    timer->start_time = SDL_GetTicks();
}

Uint32 timer_get_milliseconds(Timer *timer)
{
    return SDL_GetTicks() - timer->start_time;
}

double timer_get_seconds(Timer *timer)
{
    return timer_get_milliseconds(timer) / 1000.0;
}
