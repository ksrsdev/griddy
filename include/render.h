#ifndef GUARD_RENDER_H
#define GUARD_RENDER_H

#include "context.h"
#include "state_data.h"

void Main_Render(const GameEngine *eng, const GameData *data);
void Render_SetDrawColor(SDL_Renderer *renderer, const SDL_Color color);
void Render_SetupSDFRenderState(const GameEngine *eng, const SDL_Color color, SDL_Texture *targetTexture);
void Render_ResetRenderState(SDL_Renderer *renderer);
void Render_RectRotated(SDL_Renderer *renderer, const SDL_FRect *dest, const f64 rotation, const SDL_Color color);

void Render_ClearWhite(SDL_Renderer *renderer);

typedef void (*RenderFunc)(const GameEngine *eng, const GameData *data);

#endif
