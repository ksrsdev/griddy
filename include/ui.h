#ifndef GUARD_UI_H
#define GUARD_UI_H

#include <assert.h>

#include <SDL3/SDL.h>

#include "context.h"

typedef void (*OnClick)(GameData* data);

typedef enum {
	UI_TYPE_NONE,
	UI_TYPE_TEXT,
	UI_TYPE_TEXT_WRAPPED,
	UI_TYPE_BUTTON,
	UI_TYPE_INFO_BOX,
	UI_TYPE_COUNT
} UIType;

typedef struct {
	SDL_FRect destRect;     //16 bytes
	OnClick onClick;        //8  bytes
	f64 rotation;           //8  bytes
	SDL_Texture *texture;   //8  bytes        COULD BE MOVED INTO PARALLEL ARRAY
	SDL_Color fg;           //4  bytes
	SDL_Color bg;           //4  bytes
	SDL_Color outlineColor; //4  bytes
	u32 type;               //4  bytes
	//all bools could be folded into a u32 flags
	bool hidden;            //1  bytes       
	bool outlined;          //1  bytes
	bool isHovered;         //1  bytes       UNUSED!
	bool hasBackground;     //1  bytes       COULD BE REPLACED WITH JUST COLOR CHECK
	//Extra space before 64 byte threshold
	u8 reserved[4];         //4 bytes
} UIData;
static_assert(sizeof(UIData) == 64, "UIData not 64 bytes! include/ui.h");

bool UI_TypeHasTextWrapped(UIType type) [[unsequenced]];
void UI_UpdateHover(UIData *uiData, const FVector2 mousePos);
bool UI_CheckClick(UIData *uiData, const FVector2 mousePos);
void UI_RenderUIElement(const GameEngine *eng, const UIData *data);
void UI_SetupButton(UIData *uiData, SDL_Color fg, SDL_Color bg);
void UI_SetupDefaultButton(UIData *uiData);
void UI_SetupBackButton(UIData *uiData);

SDL_FRect UI_GetTitleDestRect(const f32 wX, const f32 wY) [[unsequenced]];
SDL_FRect UI_GetBackButtonDestRect(const f32 wX, const f32 wY) [[unsequenced]];

#endif
