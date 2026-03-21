#include "text.h"

#include <limits.h>
#include <stdio.h>

#include "context.h"
#include "types.h"


//UNUSED I THINK
SDL_Texture* Text_CreateTextureFromText(SDL_Renderer *renderer, TTF_Text *text, const s32 textW, const s32 textH)
{


//	f32 textW = 0;
//	f32 textH = 0;

//	Text_MeasureTextBounds(renderer, text, &textW, &textH);

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

	//declare outside loop for surface thing later
	s32 bestWrap = 0;

	if (destRect!= NULL) {
		f32 padding = 4.0f;
		
		f32 rectW = destRect->w - (padding * 2.0f);
		f32 rectH = destRect->h - (padding * 2.0f);
		
		f32 destRatio = rectW / rectH;
		s32 low = 64;
		s32 high = 6400;
		bestWrap = high;
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

	s32 textW = 0; 
	s32 textH = 0; 
	//Only certain UI Types need to be "certainly" centered - buttons etc
	//For now it's fine for testing but not a good idea for ALL text
	
	SDL_Color black = {0, 0, 0, 255};

	SDL_Surface *surf = TTF_RenderText_Blended_Wrapped(eng->font, textObject->text, 0, black, bestWrap);
	if (!surf) {
		printf("surf not created IDK");
		return NULL;
	}

	// 1. Get the format details from the surface
	const SDL_PixelFormatDetails *details = SDL_GetPixelFormatDetails(surf->format);

	s32 minX = surf->w, maxX = -1;
	s32 minY = surf->h, maxY = -1;

	for (s32 y = 0; y < surf->h; y++) {
		// SDL3: Jump to the start of the row using pitch
		u8 *row_ptr = (u8 *)surf->pixels + (y * surf->pitch);

		for (s32 x = 0; x < surf->w; x++) {
			u8 r, g, b, a;
			u32 pixel;

			// Determine how many bytes per pixel (usually 4)
			if (details->bytes_per_pixel == 4) {
				pixel = *(u32 *)(row_ptr + (x * 4));
			} else if (details->bytes_per_pixel == 1) {
				pixel = *(u8 *)(row_ptr + x);
			} else {
				// Handle other formats if necessary, but TTF is usually 32-bit
				continue; 
			}

			// SDL3 Version of GetRGBA
			// We pass NULL for palette because 32-bit surfaces don't use them
			SDL_GetRGBA(pixel, details, NULL, &r, &g, &b, &a);

			if (a > 0) {
				if (x < minX) minX = x;
				if (x > maxX) maxX = x;
				if (y < minY) minY = y;
				if (y > maxY) maxY = y;
			}
		}
	}

	// Final check to see if we found anything
	if (maxX == -1) {
		printf("Surface was empty (all transparent).\n");
	} else {
		printf("ASSIGNMENT!\n");
		textW = maxX - minX + 1;
		textH = maxY - minY + 1;
	}

	printf("w: %d\nh: %d\n", textW, textH);
	
	
	//int textW, textH;
	//TTF_GetTextSize(textObject, &textW, &textH);

	
	// 1. Add padding so the SDF edges don't get clipped
	int padding = 4; 
	minX = (minX - padding < 0) ? 0 : minX - padding;
	minY = (minY - padding < 0) ? 0 : minY - padding;
	maxX = (maxX + padding >= surf->w) ? surf->w - 1 : maxX + padding;
	maxY = (maxY + padding >= surf->h) ? surf->h - 1 : maxY + padding;

	int tightW = maxX - minX + 1;
	int tightH = maxY - minY + 1;

	// 2. Create a SMALL surface that is exactly the size of the text
	SDL_Surface *tightSurf = SDL_CreateSurface(tightW, tightH, surf->format);
	if (!tightSurf) return NULL;

	// 3. Blit (Copy) from the big fuzzy surface to the small tight one
	// This is where 'minX' and 'minY' are finally used to "crop" the image
	SDL_Rect srcRect = { minX, minY, tightW, tightH };
	SDL_BlitSurface(surf, &srcRect, tightSurf, NULL);

	// 4. Create the texture from the TIGHT surface
	SDL_Texture *texture = SDL_CreateTextureFromSurface(eng->renderer, tightSurf);

	// Clean up everything
	SDL_DestroySurface(tightSurf);
	SDL_DestroySurface(surf);
	TTF_DestroyText(textObject);

	return texture;
	
//	SDL_Texture *texture = SDL_CreateTexture(eng->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (s32)textW, (s32)textH);
//	if (!texture) {
//		TTF_DestroyText(textObject);
//		SDL_Log("CreateTextureFromText failed: %s", SDL_GetError());
//		return NULL;
//	}
//
//	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); 
//	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR); 
//
//	//Switch eng->renderer to texture
//	SDL_SetRenderTarget(eng->renderer, texture);
//
//	//Clear texture
//	SDL_SetRenderDrawColor(eng->renderer, 0, 0, 0, 0);
//	SDL_RenderClear(eng->renderer);
//	
//	//Draw text to texture
//	SDL_SetRenderDrawColor(eng->renderer, 255, 255, 255, 255);
//
//	TTF_DrawRendererText(textObject, (f32)-minX, (f32)-minY);
//
//	//return eng->renderer to window
//	SDL_SetRenderTarget(eng->renderer, NULL);
//
//	TTF_DestroyText(textObject);
//
//	return texture;
}

