#include "match_coin_toss.h"

#include <stdlib.h>

#include "match.h"
#include "ui.h"

static void Match_CoinToss_CleanupTextures(CoinTossData *data);

static void PreGameConfirm_InitUIStrings(CoinTossData *data);

//INIT
void Match_CoinToss_Init(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;

	matchCtx->matchStateData = calloc(1, sizeof(CoinTossData));
	if (matchCtx->matchStateData == nullptr) {
		Error_Alert(data, ERROR_ALLOC, "CoinTossData failed calloc()");
		return;
	}

	CoinTossData *coinTossData = matchCtx->matchStateData;

	Match_CoinToss_InitUIStrings(coinTossData);
	
	//UI Data
	(void)eng;

}

void Match_CoinToss_Cleanup(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;

	CoinTossData *coinTossData = matchCtx->matchStateData;

	for (s32 i = MATCH_COIN_TOSS_UI_START; i < MATCH_COIN_TOSS_UI_END; i++) {
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
void Match_CoinToss_Update(GameData *data)
{
	(void)data;
}

//RENDER
void Match_CoinToss_Render(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;
}

static void PreGameConfirm_InitUIStrings(CoinTossData *data)
{
	data->uiStrings[x]
}
