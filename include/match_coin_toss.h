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

	MATCH_COIN_TOSS_COUNT
} MatchCoinTossUIElement;

void Match_CoinToss_CleanupTextures(MatchData *data);
	
void Match_CoinToss_InitUIStrings(MatchData *data);

#endif
