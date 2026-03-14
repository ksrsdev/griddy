#ifndef GUARD_RENDER_H
#define GUARD_RENDER_H

#include "context.h"

void Main_Render(const GameEngine *eng, const GameData *data);
void Render_SetDrawColor(SDL_Renderer *renderer, const SDL_Color color);
void Render_SetupSDFRenderState(const GameEngine *eng, const SDL_Color color, SDL_Texture *targetTexture);
void Render_ResetRenderState(SDL_Renderer *renderer);

typedef void (*RenderFunc)(const GameEngine *eng, const GameData *data);

#endif
