#ifndef GUARD_MATCH_COIN_TOSS_H
#define GUARD_MATCH_COIN_TOSS_H

#include "match.h"

typedef enum {
	MATCH_COIN_TOSS_UI_NONE,
	
	MATCH_COIN_TOSS_UI_TITLE,

	MATCH_COIN_TOSS_UI_INFO_BOX,

	MATCH_COIN_TOSS_UI_INFO_BOX_TITLE,         // Call it | It's X
	MATCH_COIN_TOSS_UI_INFO_BOX_FLAVOR,        // NULL    | Which Way | CPU chose to begin on offense | Player chose to begin on defense etc
	MATCH_COIN_TOSS_UI_INFO_BOX_BUTTON_LEFT,   // HEADS   | Offense
	MATCH_COIN_TOSS_UI_INFO_BOX_BUTTON_RIGHT,  // Tails   | Defense
	MATCH_COIN_TOSS_UI_INFO_BOX_BUTTON_CENTER, // Start game

	MATCH_COIN_TOSS_UI_QUIT,

	MATCH_COIN_TOSS_COUNT,

	//Aliases
	
	MATCH_COIN_TOSS_UI_BUTTON_START = MATCH_COIN_TOSS_UI_INFO_BOX_BUTTON_LEFT,
	MATCH_COIN_TOSS_UI_BUTTON_END   = MATCH_COIN_TOSS_UI_INFO_BOX_BUTTON_CENTER + 1,
	
	MATCH_COIN_TOSS_UI_START = MATCH_COIN_TOSS_UI_NONE + 1,
	MATCH_COIN_TOSS_UI_END   = MATCH_COIN_TOSS_UI_COUNT
} MatchCoinTossUIElement;

typedef struct {
	UIData coinTossUIData[MATCH_COIN_TOSS_UI_COUNT];
	const char *coinTossStrings[MATCH_COIN_TOSS_UI_COUNT];
} CoinTossUI;

void Match_CoinToss_Init(MatchData *data);
void Match_CoinToss_Cleanup(MatchData *data);
void Match_CoinToss_Update(GameData *data);
void Match_CoinToss_Render(const GameEngine *eng, const GameData *data);
	
void Match_CoinToss_InitUIStrings(MatchData *data);

#endif
