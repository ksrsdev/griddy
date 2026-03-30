#ifndef GUARD_COLORS_h
#define GUARD_COLORS_h

#include <SDL3/SDL.h>

constexpr SDL_Color COLOR_BLACK  = {0, 0, 0, 255};

constexpr SDL_Color COLOR_WHITE  = {255, 255, 255, 255};

constexpr SDL_Color COLOR_GREEN  = {0, 204, 0, 255};

constexpr SDL_Color COLOR_RED    = {255, 0, 0, 255};

constexpr SDL_Color COLOR_PINK   = {255, 0, 255, 255};

constexpr SDL_Color COLOR_BROWN  = {102, 51, 0, 255};

constexpr SDL_Color COLOR_YELLOW = {255, 255, 0, 255};

constexpr SDL_Color COLOR_ORANGE = {255, 128, 0, 255};

constexpr SDL_Color COLOR_BLUE   = {0, 0, 255, 255};

constexpr SDL_Color COLOR_GREY   = {105, 105, 105, 255};

constexpr SDL_Color COLOR_NONE   = {0, 0, 0, 0};

bool Colors_AreEqual(SDL_Color color1, SDL_Color color2) [[unsequenced]] ;

#endif
