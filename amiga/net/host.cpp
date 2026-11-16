// game/sdl/host.cpp for GFOS: HostSleep() waits with select(0, ...), a
// socket call on AmigaOS; SDL_Delay() does the same wait without one.
#include <sys/time.h>
#include <SDL.h>
static inline int amiga_sleep_select(struct timeval *ptv)
{
    SDL_Delay((Uint32)(ptv->tv_sec * 1000 + ptv->tv_usec / 1000));
    return 0;
}
#define select(n, r, w, e, t) amiga_sleep_select(t)
#include "game/sdl/host.cpp"
