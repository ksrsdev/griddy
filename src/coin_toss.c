#include "coin_toss.h"

#include <stdlib.h>

#include "colors.h"
#include "error.h"
#include "match.h"
#include "ui.h"

static void CoinToss_Init_UI(GameEngine *eng, GameData *data);
static void CoinToss_Init_UIStrings(CoinTossData *data);
static void CoinToss_Init_UIData(CoinTossData *data);
static void CoinToss_Init_OnClickFuncs(CoinTossData *data);
static void CoinToss_Init_UITextures(GameEngine *eng, CoinTossData *data);

//INIT
void CoinToss_Init(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;

	matchCtx->matchStateData = calloc(1, sizeof(CoinTossData));
	if (matchCtx->matchStateData == nullptr) {
		Error_Alert(data, ERROR_ALLOC, "CoinTossData failed calloc()");
		return;
	}

	CoinToss_Init_UI(eng, data);
	
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

//Index for all Init_UI functions
static void CoinToss_Init_UI(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	CoinTossData *coinTossData = matchCtx->matchStateData;
	
	CoinToss_Init_UIStrings(coinTossData);

	CoinToss_Init_UIData(coinTossData);

	CoinToss_Init_OnClickFuncs(coinTossData);
	
	CoinToss_Init_UITextures(eng, coinTossData);

	//Funcs below don't strictly need to be wrapped inside Init_UI
	//They do need to be called at least once before Update

	CoinToss_ResizeLayout(coinTossData->uiData, data->window.size);

	CoinToss_CheckButtonHighlight(coinTossData->uiData, data->mouse.pos);

}

static void CoinToss_Init_UIStrings(CoinTossData *data)
{
	data->uiStrings[COIN_TOSS_UI_TITLE] = "COIN TOSS";

	data->uiStrings[COIN_TOSS_UI_INFO_BOX_TITLE] = "CALL IT";
	
	data->uiStrings[COIN_TOSS_UI_INFO_BOX_BUTTON_LEFT] = "HEADS";
	data->uiStrings[COIN_TOSS_UI_INFO_BOX_BUTTON_RIGHT] = "TAILS";
	data->uiStrings[COIN_TOSS_UI_INFO_BOX_BUTTON_CENTER] = "PLAY";

	data->uiStrings[COIN_TOSS_UI_QUIT] = "QUIT";
}

static void CoinToss_Init_UIData(CoinTossData *data)
{
	UIData *ui = nullptr;

	//Title
	ui = &data->uiData[COIN_TOSS_UI_TITLE];
	ui->type = UI_TYPE_TEXT;
	ui->fg = COLOR_BLACK;

	//Info Box
	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX];
	ui->type = UI_TYPE_INFO_BOX;
	ui->bg = COLOR_WHITE;
	ui->hasBackground = true;
	ui->outlineColor = COLOR_BLACK;
	ui->outlined = true;

	//Info Box Title
	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX_TITLE];
	ui->type = UI_TYPE_TEXT;
	ui->fg = COLOR_BLACK;

	//Info Box Flavor / Desc / Line 2
	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX_LINE2];
	ui->type = UI_TYPE_TEXT;
	ui->fg = COLOR_BLACK;

	//Info Box Button L
	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX_BUTTON_LEFT];
	ui->type = UI_TYPE_TEXT;
	ui->fg = COLOR_BLACK;
	ui->bg = COLOR_BLUE;
	ui->hasBackground = true;

	//Info Box Button R
	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX_BUTTON_RIGHT];
	ui->type = UI_TYPE_TEXT;
	ui->fg = COLOR_BLACK;
	ui->bg = COLOR_RED;
	ui->hasBackground = true;

	//Info Box Button C
	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX_BUTTON_CENTER];
	ui->type = UI_TYPE_TEXT;
	ui->fg = COLOR_BLACK;
	ui->bg = COLOR_GREEN;
	ui->hasBackground = true;

	//Quit
	ui = &data->uiData[COIN_TOSS_UI_QUIT];
	UI_SetupBackButton(ui);

}
