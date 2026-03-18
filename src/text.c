#include "text.h"

#include <stdio.h>

#include "context.h"
#include "types.h"

SDL_Texture* CreateTextureFromText(SDL_Renderer *renderer, TTF_Text *text)
{
	int textW, textH;
	TTF_GetTextSize(text, &textW, &textH);

	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, textW, textH);
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); // CRITICAL: Copy raw data, don't blend!
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

void Text_SetColor(TTF_Text *text, SDL_Color color)
{
	if (!text) {
		//ERROR
		return;
	}

	TTF_SetTextColor(text, color.r, color.g, color.b, color.a);
}

void Text_DrawCentered(TTF_Text *text, SDL_FRect *destRect)
{
	Vector2 textSize = {0};
	TTF_GetTextSize(text, &textSize.x, &textSize.y);

	FVector2 textPos = {0};
	textPos.x = destRect->x + ((destRect->w - (float)textSize.x) * 0.5f);
	textPos.y = destRect->y + ((destRect->h - (float)textSize.y) * 0.5f);

	TTF_DrawRendererText(text, textPos.x, textPos.y);
}

SDL_Texture* Text_CreateTextTexture(const GameEngine *eng, const char *string, const SDL_FRect *destRect)
{

	if (!string || string[0] == '\0') {
		SDL_Log("Text_CreateTextTexture string invalid!");
		return NULL;
	}

	TTF_Text *textObject = TTF_CreateText(eng->textEngine, eng->font, string, 0);
	if (!textObject) {
		SDL_Log("TTF_CreateText failed: %s", SDL_GetError());
		return NULL;
	}

	if (destRect!= NULL) {
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
		
		TTF_SetTextWrapWidth(textObject, (s32)bestWrap);
	}

	SDL_Texture *texture = CreateTextureFromText(eng->renderer, textObject);
	if (!texture) {
		TTF_DestroyText(textObject);
		SDL_Log("CreateTextureFromText failed: %s", SDL_GetError());
		return NULL;
	}

	TTF_DestroyText(textObject);

	return texture;
}

