#include "match_coin_toss.h"

#include "ui.h"

static void Match_CoinToss_CleanupTextures(MatchData *data);

void Match_CoinToss_Init(MatchData *data)
{
	//Strings
	//UI Data

}

void Match_CoinToss_Cleanup(MatchData *data)
{
	Match_CoinToss_CleanupTextures(data);

	//Clear

}

static void Match_CoinToss_CleanupTextures(MatchData *data)
{
	for (s32 i = MATCH_COIN_TOSS_UI_START; i < MATCH_COIN_TOSS_UI_END; i++) {
		UIData *uiData = &data->uiData[i];
		if (uiData->texture) {
			SDL_DestroyTexture(uiData->texture);
			uiData->texture = nullptr;
		}
}
