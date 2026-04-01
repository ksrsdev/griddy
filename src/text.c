#include "text.h"

#include <limits.h>
#include <stdio.h>

#include "context.h"
#include "types.h"

static s32 Text_GetBestWrap(TTF_Text *textObject, SDL_FRect *destRect);

//LEGACY - Intro.c
SDL_Texture* Text_CreateTextureFromText(SDL_Renderer *renderer, TTF_Text *text, const s32 textW, const s32 textH)
{

	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (s32)textW, (s32)textH);

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); 
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR); 

	//Switch renderer to texture
	SDL_SetRenderTarget(renderer, texture);

	//Clear texture
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	
	//Draw text to texture
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	TTF_DrawRendererText(text, 0, 0);

	//return renderer to window
	SDL_SetRenderTarget(renderer, NULL);

	return texture;
}

//LEGACY - intro.c
void Text_SetColor(TTF_Text *text, SDL_Color color)
{
	if (!text) {
		//ERROR
		return;
	}

	TTF_SetTextColor(text, color.r, color.g, color.b, color.a);
}

SDL_Texture * Text_CreateUITexture(const GameEngine *eng, const char *string, UIData *uiData)
{

	if (!string || string[0] == '\0') {
		return nullptr;
	}

	TTF_Text *textObject = TTF_CreateText(eng->textEngine, eng->font, string, 0);
	if (!textObject) {
		SDL_Log("TTF_CreateText failed: %s", SDL_GetError());
		return nullptr;
	}
	
	if (UI_TypeHasTextWrapped(uiData->type)) {
		s32 bestWrap = Text_GetBestWrap(textObject, &uiData->destRect);
		TTF_SetTextWrapWidth(textObject, bestWrap);
	}

	s32 textW = 0; 
	s32 textH = 0;
	TTF_GetTextSize(textObject, &textW, &textH);
	
	SDL_Texture *texture = SDL_CreateTexture(eng->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (s32)textW, (s32)textH);
	if (!texture) {
		TTF_DestroyText(textObject);
		SDL_Log("CreateTextureFromText failed: %s", SDL_GetError());
		return nullptr;
	}
		
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); 
	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR); 

	//Switch eng->renderer to texture
	SDL_SetRenderTarget(eng->renderer, texture);

	//Clear texture
	SDL_SetRenderDrawColor(eng->renderer, 0, 0, 0, 0);
	SDL_RenderClear(eng->renderer);
	
	SDL_SetRenderDrawColor(eng->renderer, 255, 255, 255, 255);

	f32 bestX = 0;
	if (!UI_TypeHasTextWrapped(uiData->type)) {
		bestX = 8;
	}

	TTF_DrawRendererText(textObject, bestX, 8);
		
	SDL_SetRenderTarget(eng->renderer, NULL);
	TTF_DestroyText(textObject);

	return texture;
}

static s32 Text_GetBestWrap(TTF_Text *textObject, SDL_FRect *destRect)
{
	f32 padding = 4.0f;
		
	f32 rectW = destRect->w - (padding * 2.0f);
	f32 rectH = destRect->h - (padding * 2.0f);
		
	f32 destRatio = rectW / rectH;
	s32 low = 64;
	s32 high = 6400;
	s32 bestWrap = high;
	f32 bestDiff = 1e6;

	for (u8 i = 0; i < 10; i++) {

		s32 mid = (low + high) / 2;
		
		TTF_SetTextWrapWidth(textObject, mid);

		s32 w, h;
		TTF_GetTextSize(textObject, &w, &h);
		f32 currRatio = (float)w / (float)h;

		f32 diff = SDL_fabsf(currRatio - destRatio);
		if (diff < bestDiff) {
			bestDiff = diff;
			bestWrap = mid;
		}

		if (currRatio > destRatio) {
			high = mid;
		} else if (currRatio < destRatio) {
			low = mid;
		} else {
			break;
		}
	}

	return bestWrap;
}
