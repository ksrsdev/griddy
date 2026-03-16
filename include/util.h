#ifndef GUARD_UTIL_H
#define GUARD_UTIL_H

#include <SDL3/SDL.h>

#define DEG_TO_RAD (SDL_PI_F / 180.0f)
#define RAD_TO_DEG (180.0f / SDL_PI_F)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#endif
