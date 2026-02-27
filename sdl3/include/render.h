#ifndef GUARD_RENDER_H
#define GUARD_RENDER_H

#include "context.h"

void RenderCore(const GameEngine *eng, const GameData *data);
void Render_SetDrawColor(SDL_Renderer *renderer, const SDL_Color color);

#endif
