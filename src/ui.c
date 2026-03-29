#include "ui.h"

#include <stdio.h>

#include "colors.h"
#include "render.h"
#include "util.h"

static void UI_RenderElementOutline(SDL_Renderer *renderer, const UIData *data);
static bool UI_TypeHasHoverHighlight(UIType type);

void UI_RenderUIElement(const GameEngine *eng, const UIData *data)
{

	//outline
	if (data->outlined && data->outlineColor.a != 0) {
		UI_RenderElementOutline(eng->renderer, data);
	}
	
	//bg
	if (data->hasBackground && data->bg.a != 0) {
		if (data->rotation == 0) {
			Render_SetDrawColor(eng->renderer, data->bg);
			SDL_RenderFillRect(eng->renderer, &data->destRect);
		} else {
			Render_RectRotated(eng->renderer, &data->destRect, data->rotation, data->bg);
		}
	}

	//fg = text
	
	if (!data->texture) {
		//ERROR
		return;
	}
	SDL_Texture *texture = data->texture;
	
	f32 padding = 4.0f;

	SDL_FRect paddedRect;
	paddedRect.x = data->destRect.x + padding;
	paddedRect.y = data->destRect.y + padding;
	// Ensure we don't collapse the box into a black hole
	paddedRect.w = MAX(0.0f, data->destRect.w - (padding * 2.0f));
	paddedRect.h = MAX(0.0f, data->destRect.h - (padding * 2.0f));

	f32 scaleX = paddedRect.w / (f32)texture->w;
	f32 scaleY = paddedRect.h / (f32)texture->h;

	f32 finalScale = MIN(scaleX, scaleY);

	SDL_FRect targetRect;
	targetRect.w = (f32)texture->w * finalScale;
	targetRect.h = (f32)texture->h * finalScale;
	targetRect.x = paddedRect.x + ((paddedRect.w - targetRect.w) * 0.5f);
	targetRect.y = paddedRect.y + ((paddedRect.h - targetRect.h) * 0.5f);

	Render_SetupSDFRenderState(eng, data->fg, texture);
	if (data->rotation == 0) {
		SDL_RenderTexture(eng->renderer, texture, NULL,  &targetRect);
	} else {
		SDL_RenderTextureRotated(eng->renderer, texture, NULL, &targetRect, data->rotation, NULL, SDL_FLIP_NONE);
	}
	Render_ResetRenderState(eng->renderer);
}

#define OUTLINE_PIXEL_WIDTH 4.0f

static void UI_RenderElementOutline(SDL_Renderer *renderer, const UIData *data)
{
	SDL_FRect outlineRect;
	outlineRect.w = data->destRect.w + (2.0f * OUTLINE_PIXEL_WIDTH);
	outlineRect.h = data->destRect.h + (2.0f * OUTLINE_PIXEL_WIDTH);
	outlineRect.x = data->destRect.x - OUTLINE_PIXEL_WIDTH;
	outlineRect.y = data->destRect.y - OUTLINE_PIXEL_WIDTH;
	Render_SetDrawColor(renderer, data->outlineColor);
	SDL_RenderFillRect(renderer, &outlineRect);
}

#undef OUTLINE_PIXEL_WIDTH

bool UI_TypeHasTextWrapped(UIType type)
{
	if (
			(type == UI_TYPE_TEXT_WRAPPED)
	   ) {
		return true;
	} else {
		return false;
	}
}

void UI_UpdateHover(UIData *uiData, const FVector2 mousePos)
{
	//Only need to check mosuePos for certain types
	if (!UI_TypeHasHoverHighlight(uiData->type)) {
		return;
	}
	
	if (
			mousePos.x >= uiData->destRect.x &&
			mousePos.x <= uiData->destRect.x + uiData->destRect.w &&
			mousePos.y >= uiData->destRect.y &&
			mousePos.y <= uiData->destRect.y + uiData->destRect.h 
	   ) {
		uiData->isHovered = true;
		uiData->outlined = true;
	} else {
		uiData->isHovered = false;
		uiData->outlined = false;
	}
}

static bool UI_TypeHasHoverHighlight(UIType type)
{
	if (
			(type == UI_TYPE_BUTTON)
	   ) {
		return true;
	} else {
		return false;
	}
}

bool UI_CheckClick(UIData *uiData, const FVector2 mousePos)
{
	if (
			mousePos.x >= uiData->destRect.x &&
			mousePos.x <= uiData->destRect.x + uiData->destRect.w &&
			mousePos.y >= uiData->destRect.y &&
			mousePos.y <= uiData->destRect.y + uiData->destRect.h 
	   ) {
		return true;
	} else {
		return false;
	}
}

void UI_SetupButton(UIData *uiData, SDL_Color fg, SDL_Color bg)
{
	uiData->type          = UI_TYPE_BUTTON;
	uiData->fg            = fg;
	uiData->bg            = bg;
	uiData->hasBackground = true;
	if (bg == COLOR_BLACK) {
		uiData->outlineColor = COLOR_RED;
	} else {
		uiData->outlineColor  = COLOR_BLACK;
	}
}

void UI_SetupDefaultButton(UIData *uiData)
{
//	uiData->type          = UI_TYPE_BUTTON;
//	uiData->fg            = COLOR_BLACK;
//	uiData->bg            = COLOR_GREY;
//	uiData->hasBackground = true;
//	uiData->outlineColor  = COLOR_BLACK;

	UI_SetupButton(uiData, COLOR_BLACK, COLOR_GREY);
}

void UI_SetupBackButton(UIData *uiData)
{
//	uiData->type          = UI_TYPE_BUTTON;
//	uiData->fg            = COLOR_BLACK;
//	uiData->bg            = COLOR_RED;
//	uiData->hasBackground = true;
//	uiData->outlineColor  = COLOR_BLACK;

	UI_SetupButton(uiData, COLOR_BLACK, COLOR_RED);
}

SDL_FRect UI_GetTitleDestRect(const f32 wX, const f32 wY)
{
	SDL_FRect dest = {0};

	dest.w = wX * 0.5f;
	dest.h = wY * 0.20f;
	dest.x = (wX - dest.w) * 0.50f;
	dest.y = wY * 0.10f;

	return dest;
}

SDL_FRect UI_GetBackButtonDestRect(const f32 wX, const f32 wY)
{
	SDL_FRect dest = {0};

	dest.w = wX * 0.1f;
	dest.h = wY * 0.1f;
	dest.x = wX * 0.05f;
	dest.y = wY - dest.h - (wY * 0.05f);

	return dest;
}
