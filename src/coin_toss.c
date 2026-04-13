#include "coin_toss.h"

#include <stdlib.h>

#include "error.h"
#include "match.h"
#include "ui.h"

static void CoinToss_Init_UIStrings(CoinTossData *data);

//INIT
void CoinToss_Init(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;

	matchCtx->matchStateData = calloc(1, sizeof(CoinTossData));
	if (matchCtx->matchStateData == nullptr) {
		Error_Alert(data, ERROR_ALLOC, "CoinTossData failed calloc()");
		return;
	}

	CoinTossData *coinTossData = matchCtx->matchStateData;

	CoinToss_Init_UIStrings(coinTossData);
	
	//UI Data
	(void)eng;

}

void CoinToss_Cleanup(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;

	CoinTossData *coinTossData = matchCtx->matchStateData;

	for (s32 i = COIN_TOSS_UI_START; i < COIN_TOSS_UI_END; i++) {
		UIData *uiData = &coinTossData->uiData[i];
		if (uiData->texture) {
			SDL_DestroyTexture(uiData->texture);
			uiData->texture = nullptr;
		}
	}

	//the sub-state data pointer is freed and cleared by Match_Cleanup_MatchStateData()
	
	(void)eng;

}

//UPDATE
void CoinToss_Update(GameData *data)
{
	(void)data;
}

//RENDER
void CoinToss_Render(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;
}

static void CoinToss_Init_UIStrings(CoinTossData *data)
{
	(void)data;
}
