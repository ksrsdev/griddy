#ifndef GUARD_UI_H
#define GUARD_UI_H

#include <assert.h>
#include <stdbool.h>

#include <SDL3/SDL.h>

#include "button.h"

typedef enum {
	UI_TYPE_NONE,
	UI_TYPE_TEXT,
	UI_TYPE_TEXT_WRAPPED,
	UI_TYPE_BUTTON,
	UI_TYPE_COUNT
} UIType;

typedef struct {
	SDL_FRect destRect;     //16 bytes
	OnClick onClick;        //8  bytes
	f64 rotation;           //8  bytes
	SDL_Texture *texture;   //8  bytes
	SDL_Color fg;           //4  bytes
	SDL_Color bg;           //4  bytes
	SDL_Color outlineColor; //4  bytes
	u32 type;               //4  bytes
	bool hidden;            //1  bytes
	bool outlined;          //1  bytes
	bool isHovered;         //1  bytes
	bool hasBackground;     //1  bytes
	//Extra space before 64 byte threshold
	u8 reserved[4];         //4 bytes
} UIData;

static_assert(sizeof(UIData) == 64, "UIData not 64 bytes! include/ui.h");

bool Text_UITypeHasTextWrapped(UIType type);

#endif
