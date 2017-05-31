#ifndef _TIMER_H_INCLUDED_
#define _TIMER_H_INCLUDED_

#include <SDL.h>

typedef struct Timer Timer;

struct Timer
{
    Uint32 start_time;
};

void timer_start(Timer *timer);
Uint32 timer_get_milliseconds(Timer *timer);
double timer_get_seconds(Timer *timer);

#endif
