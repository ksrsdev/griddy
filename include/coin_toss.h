#ifndef GUARD_COIN_TOSS_H
#define GUARD_COIN_TOSS_H

#include "context.h"

#include "ui.h"

typedef enum {
	COIN_TOSS_UI_NONE,
	
	COIN_TOSS_UI_TITLE,

	COIN_TOSS_UI_INFO_BOX,

	COIN_TOSS_UI_INFO_BOX_TITLE,         // Call it | It's X
	COIN_TOSS_UI_INFO_BOX_LINE2,         // NULL    | Which Way | CPU chose to begin on offense | Player chose to begin on defense etc
	COIN_TOSS_UI_INFO_BOX_BUTTON_LEFT,   // HEADS   | Offense
	COIN_TOSS_UI_INFO_BOX_BUTTON_RIGHT,  // Tails   | Defense
	COIN_TOSS_UI_INFO_BOX_BUTTON_CENTER, // Start game

	COIN_TOSS_UI_QUIT,

	COIN_TOSS_UI_COUNT,

	//Aliases
	
	COIN_TOSS_UI_BUTTON_START = COIN_TOSS_UI_INFO_BOX_BUTTON_LEFT,
	COIN_TOSS_UI_BUTTON_END   = COIN_TOSS_UI_INFO_BOX_BUTTON_CENTER + 1,
	
	COIN_TOSS_UI_START = COIN_TOSS_UI_NONE + 1,
	COIN_TOSS_UI_END   = COIN_TOSS_UI_COUNT
} CoinTossUIElement;

typedef struct {
	//UI stuff
	UIData uiData[COIN_TOSS_UI_COUNT];
	const char *uiStrings[COIN_TOSS_UI_COUNT];
	//state specific vars etc
} CoinTossData;

void CoinToss_Init(GameEngine *eng, GameData *data);
void CoinToss_Cleanup(GameEngine *eng, GameData *data);
void CoinToss_Update(GameData *data);
void CoinToss_Render(const GameEngine *eng, const GameData *data);
	
#endif
