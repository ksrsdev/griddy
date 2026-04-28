#include "coin_toss.h"

#include <stdlib.h>
#include <stdio.h>

#include "colors.h"
#include "error.h"
#include "match.h"
#include "render.h"
#include "text.h"
#include "ui.h"
#include "update.h"

static void CoinToss_Init_UI(GameEngine *eng, GameData *data);

static void CoinToss_Init_UIStrings(CoinTossData *data);
static void CoinToss_Init_UIData(CoinTossData *data);
static void CoinToss_Init_UIOnClickFuncs(CoinTossData *data);
static void CoinToss_Init_UITextures(GameEngine *eng, CoinTossData *data);

static void CoinToss_ResizeLayout(UIData *uiData, const Vector2 windowSize);
static void CoinToss_ResizeInfoBoxMembers(UIData *data);

static void CoinToss_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos);
static CoinTossUIElement CoinToss_CheckButtonClick(UIData *uiData, const FVector2 mousePos);

static void CoinToss_ButtonLeft_OnClick(GameData *data);
static void CoinToss_ButtonRight_OnClick(GameData *data);
static void CoinToss_ButtonCenter_OnClick(GameData *data);
static void CoinToss_Quit_OnClick(GameData *data);
	
static void CoinToss_UpdateStrings_ResultPhase(MatchCtx *matchCtx);
static void CoinToss_UpdateStrings_InfoBoxTitle(CoinTossData *data);

static const char* CoinToss_GetCoinResultString(const CoinResult coin) [[unsequenced]];

static void CoinToss_UpdateTextures(GameEngine *eng, CoinTossData *data);

static void CoinToss_ResolveToss(MatchCtx *matchCtx, CoinResult coinGuess);

static void CoinToss_CPUElect(MatchCtx *matchCtx);
static void CoinToss_ElectOff(MatchCtx *matchCtx);
static void CoinToss_ElectDef(MatchCtx *matchCtx);

static void CoinToss_SetupElect(CoinTossData *data);
static void CoinToss_SetupResult(MatchCtx *matchCtx);

//INIT
void CoinToss_Init(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;

	matchCtx->matchStateData = calloc(1, sizeof(CoinTossData));
	if (matchCtx->matchStateData == nullptr) {
		Error_Alert(data, ERROR_ALLOC, "CoinTossData failed calloc()");
		return;
	}

	//Init Coin Toss Phases
	CoinTossData *coinTossData = matchCtx->matchStateData;

	coinTossData->phase.curr = COIN_TOSS_PHASE_CALL_COIN;
	coinTossData->phase.next = COIN_TOSS_PHASE_CALL_COIN;

	//Init Coin Toss UI
	CoinToss_Init_UI(eng, data);
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
	MatchCtx *matchCtx = data->stateData;
	CoinTossData *coinTossData = matchCtx->matchStateData;

	if (data->window.resized) {
		CoinToss_ResizeLayout(coinTossData->uiData, data->window.size);
	}
	
	if (data->mouse.moved) {
		CoinToss_CheckButtonHighlight(coinTossData->uiData, data->mouse.pos);
	}
	
	if (data->mouse.left.wasPressed) {
		CoinTossUIElement clicked = CoinToss_CheckButtonClick(coinTossData->uiData, data->mouse.pos);

		if (clicked != COIN_TOSS_UI_NONE) {
			UIData dataClicked = coinTossData->uiData[clicked];
			if (dataClicked.onClick) {
				OnClick onClick = dataClicked.onClick;
				onClick(data);
			}
		}
	}
	
}

//POST UPDATE
void CoinToss_PostUpdate(GameEngine *eng, MatchCtx *matchCtx)
{
	CoinTossData *coinTossData = matchCtx->matchStateData;

	if (coinTossData->phase.curr != coinTossData->phase.next) {
		CoinToss_UpdateTextures(eng, coinTossData);
		coinTossData->phase.curr = coinTossData->phase.next;
	}

}

//RENDER
void CoinToss_Render(const GameEngine *eng, const GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	CoinTossData *coinTossData = matchCtx->matchStateData;

	Render_ClearWhite(eng->renderer);
	
	for (s32 i = COIN_TOSS_UI_START; i < COIN_TOSS_UI_END; i++) {
		UIData *uiData = &coinTossData->uiData[i];
		UI_RenderUIElement(eng, uiData);
	}
}

//Index for all Init_UI functions
static void CoinToss_Init_UI(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	CoinTossData *coinTossData = matchCtx->matchStateData;
	
	CoinToss_Init_UIStrings(coinTossData);

	CoinToss_Init_UIData(coinTossData);

	CoinToss_Init_UIOnClickFuncs(coinTossData);
	
	CoinToss_Init_UITextures(eng, coinTossData);

	//Funcs below don't strictly need to be wrapped inside Init_UI
	//They do need to be called at least once before Update

	CoinToss_ResizeLayout(coinTossData->uiData, data->window.size);

	CoinToss_CheckButtonHighlight(coinTossData->uiData, data->mouse.pos);

}

static void CoinToss_Init_UIStrings(CoinTossData *data)
{
	//Link Dynamic strings to their buffers:
	data->uiStrings[COIN_TOSS_UI_INFO_BOX_TITLE] = data->stringBuffers[COIN_TOSS_STRING_BUFFER_INFO_BOX_TITLE];
	snprintf(data->stringBuffers[COIN_TOSS_STRING_BUFFER_INFO_BOX_TITLE], sizeof(data->stringBuffers[COIN_TOSS_STRING_BUFFER_INFO_BOX_TITLE]), "CALL IT!");
	
	data->uiStrings[COIN_TOSS_UI_INFO_BOX_LINE2] = data->stringBuffers[COIN_TOSS_STRING_BUFFER_INFO_BOX_LINE2];

	//Static String assignment
	data->uiStrings[COIN_TOSS_UI_TITLE] = "COIN TOSS";
	
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
	ui->type = UI_TYPE_BUTTON;
	ui->fg = COLOR_BLACK;
	ui->bg = COLOR_RED;
	ui->hasBackground = true;
	ui->outlineColor = COLOR_BLACK;

	//Info Box Button R
	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX_BUTTON_RIGHT];
	ui->type = UI_TYPE_BUTTON;
	ui->fg = COLOR_BLACK;
	ui->bg = COLOR_BLUE;
	ui->hasBackground = true;
	ui->outlineColor = COLOR_BLACK;

	//Info Box Button C
	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX_BUTTON_CENTER];
	ui->type = UI_TYPE_BUTTON;
	ui->fg = COLOR_BLACK;
	ui->bg = COLOR_GREEN;
	ui->hasBackground = true;
	ui->outlineColor = COLOR_BLACK;
	ui->hidden = true;

	//Quit
	ui = &data->uiData[COIN_TOSS_UI_QUIT];
	UI_SetupBackButton(ui);
}

static void CoinToss_Init_UIOnClickFuncs(CoinTossData *data)
{
	UIData *ui = nullptr;

	//Button Left
	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX_BUTTON_LEFT];
	ui->onClick = CoinToss_ButtonLeft_OnClick;

	//Button Right
	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX_BUTTON_RIGHT];
	ui->onClick = CoinToss_ButtonRight_OnClick;

	//Button Center
	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX_BUTTON_CENTER];
	ui->onClick = CoinToss_ButtonCenter_OnClick;
	
	//Quit
	ui = &data->uiData[COIN_TOSS_UI_QUIT];
	ui->onClick = CoinToss_Quit_OnClick;
}

static void CoinToss_Init_UITextures(GameEngine *eng, CoinTossData *data)
{
	for (s32 i = COIN_TOSS_UI_START; i < COIN_TOSS_UI_END; i++) {
		UIData *ui = &data->uiData[i];
		ui->texture = Text_CreateUITexture(eng, data->uiStrings[i], ui);
	}
}

static void CoinToss_ResizeLayout(UIData *uiData, const Vector2 windowSize)
{
	f32 wX = (f32)windowSize.x;
	f32 wY = (f32)windowSize.y;

	SDL_FRect *dest = nullptr;

	//Title
	dest = &uiData[COIN_TOSS_UI_TITLE].destRect;

	*dest = UI_GetTitleDestRect(wX, wY);

	//Info Box
	dest = &uiData[COIN_TOSS_UI_INFO_BOX].destRect;

	dest->x = wX * 0.1f;
	dest->y = wY * 0.4f;
	dest->w = wX * 0.8f;
	dest->h = wY * 0.4f;

	//Info Box Members
	dest = nullptr;

	CoinToss_ResizeInfoBoxMembers(uiData);
	
	//Quit
	dest = &uiData[COIN_TOSS_UI_QUIT].destRect;

	*dest = UI_GetBackButtonDestRect(wX, wY);

}

static void CoinToss_ResizeInfoBoxMembers(UIData *data)
{
	SDL_FRect infoBox = data[COIN_TOSS_UI_INFO_BOX].destRect;

	f32 infoX = infoBox.x;
	f32 infoY = infoBox.y;
	f32 infoW = infoBox.w;
	f32 infoH = infoBox.h;

	SDL_FRect *dest = nullptr;

	//Title
	dest = &data[COIN_TOSS_UI_INFO_BOX_TITLE].destRect;

	dest->w = infoW * 0.8f;
	dest->h = infoH * 0.2f;
	dest->x = (infoW - dest->w) * 0.5f;
	dest->y = infoH * 0.1f;

	dest->x += infoX;
	dest->y += infoY;
	
	//Line2
	dest = &data[COIN_TOSS_UI_INFO_BOX_LINE2].destRect;

	*dest = data[COIN_TOSS_UI_INFO_BOX_TITLE].destRect;

	dest->y += infoH * 0.3f;

	//Button Row Setup
	
	SDL_FRect buttonRowArea = {};
	buttonRowArea.x = infoX + (infoW * 0.1f);
	buttonRowArea.y = infoY + (infoH * 0.7f);
	buttonRowArea.w = infoW * 0.8f;
	buttonRowArea.h = infoH * 0.2f;

	//Hardcoded - not ideal but if it never changes who cares
	s32 numButtons = 3;
	
	//Button L
	dest = &data[COIN_TOSS_UI_INFO_BOX_BUTTON_LEFT].destRect;

	dest->h = buttonRowArea.h;
	dest->y = buttonRowArea.y;
	dest->w = buttonRowArea.w * 0.8f / (f32)numButtons;
	dest->x = buttonRowArea.x;

	//Button R
	dest = &data[COIN_TOSS_UI_INFO_BOX_BUTTON_RIGHT].destRect;

	dest->h = buttonRowArea.h;
	dest->y = buttonRowArea.y;
	dest->w = buttonRowArea.w * 0.8f / (f32)numButtons;
	dest->x = (buttonRowArea.x + buttonRowArea.w - dest->w - (buttonRowArea.h * 0.1f));

	//Button C
	dest = &data[COIN_TOSS_UI_INFO_BOX_BUTTON_CENTER].destRect;

	dest->h = buttonRowArea.h;
	dest->y = buttonRowArea.y;
	dest->w = buttonRowArea.w * 0.8f / (f32)numButtons;
	dest->x = buttonRowArea.x + (buttonRowArea.w * 0.5f) - (dest->w * 0.5f);
}

static void CoinToss_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos)
{
	for (s32 i = COIN_TOSS_UI_BUTTON_START; i < COIN_TOSS_UI_BUTTON_END; i++) {
		
		UIData *data = &uiData[i];
		if (data->hidden) {
			continue;
		}
		UI_UpdateHover(data, mousePos);
	}
}

static CoinTossUIElement CoinToss_CheckButtonClick(UIData *uiData, const FVector2 mousePos)
{
	for (s32 i = COIN_TOSS_UI_BUTTON_START; i < COIN_TOSS_UI_BUTTON_END; i++) {
		 if (UI_CheckClick(&uiData[i], mousePos)) {
			 return i;
		 }
	}
	return COIN_TOSS_UI_NONE;

}

static void CoinToss_ButtonLeft_OnClick(GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	CoinTossData *coinTossData = matchCtx->matchStateData;

	switch (coinTossData->phase.curr) {
		case COIN_TOSS_PHASE_CALL_COIN:
			CoinToss_ResolveToss(matchCtx, COIN_HEADS);
			break;
		case COIN_TOSS_PHASE_ELECT:
			CoinToss_ElectOff(matchCtx);
			break;
		default:
			Error_Alert(data, ERROR_UNDEFINED_BEHAVIOR, "Coin Toss Phase OOB OnClick");
			break;
	}
}

static void CoinToss_ButtonRight_OnClick(GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	CoinTossData *coinTossData = matchCtx->matchStateData;

	switch (coinTossData->phase.curr) {
		case COIN_TOSS_PHASE_CALL_COIN:
			CoinToss_ResolveToss(matchCtx, COIN_TAILS);
			break;
		case COIN_TOSS_PHASE_ELECT:
			CoinToss_ElectDef(matchCtx);
			break;
		default:
			Error_Alert(data, ERROR_UNDEFINED_BEHAVIOR, "Coin Toss Phase OOB OnClick");
			break;
	}
}

static void CoinToss_ButtonCenter_OnClick(GameData *data)
{
	(void)data;
}

static void CoinToss_Quit_OnClick(GameData *data)
{
	RequestGameStateTransition(data, MAIN_STATE_MAIN_MENU);
}

static void CoinToss_UpdateStrings_ResultPhase(MatchCtx *matchCtx)
{
	//It is [result] - [winner] has won the call.
	//[winner] has elected to begin on [off/def]

	CoinTossData *coinTossData = matchCtx->matchStateData;

	CoinToss_UpdateStrings_InfoBoxTitle(coinTossData);

	//Info Box Line 2
	const char *sDir = nullptr;
	
	if (coinTossData->guessCorrect) {
		sDir = (matchCtx->possesion == POSSESSION_PLAYER) ? "OFFENSE" : "DEFENSE";
	} else {
		sDir = (matchCtx->possesion == POSSESSION_PLAYER) ? "DEFENSE" : "OFFENSE";
	}

	//Winner String 
	const char *sWinner = coinTossData->guessCorrect ? "PLAYER" : "CPU";
	
	snprintf(coinTossData->stringBuffers[COIN_TOSS_STRING_BUFFER_INFO_BOX_LINE2], sizeof(coinTossData->stringBuffers[COIN_TOSS_STRING_BUFFER_INFO_BOX_LINE2]), "%s has elected to begin on %s", sWinner, sDir); 
}

static void CoinToss_UpdateStrings_InfoBoxTitle(CoinTossData *data)
{
	const char *sWinner = data->guessCorrect ? "PLAYER" : "CPU";

	const char *sResult = CoinToss_GetCoinResultString(data->coinResult);
	
	snprintf(data->stringBuffers[COIN_TOSS_STRING_BUFFER_INFO_BOX_TITLE], sizeof(data->stringBuffers[COIN_TOSS_STRING_BUFFER_INFO_BOX_TITLE]), "It's %s! %s has won the toss.", sResult, sWinner); 
}

static const char* CoinToss_GetCoinResultString(const CoinResult coin) [[unsequenced]]
{
	switch (coin) {
		case COIN_HEADS:
			return "HEADS";
		case COIN_TAILS:
			return "TAILS";
		default:
			return "NULL!";
	}
}

static void CoinToss_UpdateTextures(GameEngine *eng, CoinTossData *data)
{
	for (s32 i = COIN_TOSS_UI_START; i < COIN_TOSS_UI_END; i++) {
		UIData *ui = &data->uiData[i];
		if (!ui->texture) {
			ui->texture = Text_CreateUITexture(eng, data->uiStrings[i], ui);
		}
	}
}


static void CoinToss_ResolveToss(MatchCtx *matchCtx, CoinResult coinGuess)
{
	CoinTossData *coinTossData = matchCtx->matchStateData;

	//actual flip is random
	CoinResult coinResult = COIN_NONE;
	s32 randNum = rand() % 2;

	if (randNum > 0) {
		coinResult = COIN_HEADS;
	} else {
		coinResult = COIN_TAILS;
	}
	
	//Record coinResult in coinTossDat
	coinTossData->coinResult = coinResult;

	//record player won or not and setup next coin toss phase
	if (coinGuess == coinResult) {
		coinTossData->guessCorrect = true;
		CoinToss_SetupElect(coinTossData);
	} else {
		coinTossData->guessCorrect = false;
		CoinToss_CPUElect(matchCtx);
		CoinToss_SetupResult(matchCtx);
	}
}

static void CoinToss_CPUElect(MatchCtx *matchCtx)
{
	//75% of the time CPU elects to start of offense
	s32 randNum = rand() % 20;
	
	if (randNum > 14) {
		matchCtx->possesion = POSSESSION_PLAYER;
	} else {
		matchCtx->possesion = POSSESSION_CPU;
	}

}

static void CoinToss_ElectOff(MatchCtx *matchCtx)
{
	//Set match ctx possesion
	matchCtx->possesion = POSSESSION_PLAYER;

	CoinToss_SetupResult(matchCtx);
}

static void CoinToss_ElectDef(MatchCtx *matchCtx)
{
	//Set match ctx possesion
	matchCtx->possesion = POSSESSION_CPU;

	CoinToss_SetupResult(matchCtx);
}

static void CoinToss_SetupElect(CoinTossData *data)
{
	//Layout stays the same just update the strings
	

	UIData *ui = nullptr;
	
	//Info Box Title
	CoinToss_UpdateStrings_InfoBoxTitle(data);

	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX_TITLE];
	if (ui->texture) {
		SDL_DestroyTexture(ui->texture);
		ui->texture = nullptr;
	}
	
	//Info Box Line2
	snprintf(data->stringBuffers[COIN_TOSS_STRING_BUFFER_INFO_BOX_LINE2], sizeof(data->stringBuffers[COIN_TOSS_STRING_BUFFER_INFO_BOX_LINE2]), "Would you like to begin on offense, or defense?");
	
	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX_LINE2];
	if (ui->texture) {
		SDL_DestroyTexture(ui->texture);
		ui->texture = nullptr;
	}

	//ButtonL
	data->uiStrings[COIN_TOSS_UI_INFO_BOX_BUTTON_LEFT] = "OFFENSE";
	
	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX_BUTTON_LEFT];
	if (ui->texture) {
		SDL_DestroyTexture(ui->texture);
		ui->texture = nullptr;
	}
	
	//ButtonR
	data->uiStrings[COIN_TOSS_UI_INFO_BOX_BUTTON_RIGHT] = "DEFENSE";
	
	ui = &data->uiData[COIN_TOSS_UI_INFO_BOX_BUTTON_RIGHT];
	if (ui->texture) {
		SDL_DestroyTexture(ui->texture);
		ui->texture = nullptr;
	}

	//Update next phase for PostUpdate
	data->phase.next = COIN_TOSS_PHASE_ELECT;
}

static void CoinToss_SetupResult(MatchCtx *matchCtx)
{
	CoinTossData *coinTossData = matchCtx->matchStateData;
	UIData *ui = nullptr;

	//Hide L and R buttons
	ui = &coinTossData->uiData[COIN_TOSS_UI_INFO_BOX_BUTTON_LEFT];
	ui->hidden = true;
	
	ui = &coinTossData->uiData[COIN_TOSS_UI_INFO_BOX_BUTTON_RIGHT];
	ui->hidden = true;
	
	//Show Center button
	ui = &coinTossData->uiData[COIN_TOSS_UI_INFO_BOX_BUTTON_CENTER];
	ui->hidden = false;

	//set new strings for title, info box title, info box line 2
	CoinToss_UpdateStrings_ResultPhase(matchCtx);
	
	//destroy textures 
	
	//destroy info box title
	ui = &coinTossData->uiData[COIN_TOSS_UI_INFO_BOX_TITLE];
	if (ui->texture) {
		SDL_DestroyTexture(ui->texture);
		ui->texture = nullptr;
	}

	//destroy info box line 2
	ui = &coinTossData->uiData[COIN_TOSS_UI_INFO_BOX_LINE2];
	if (ui->texture) {
		SDL_DestroyTexture(ui->texture);
		ui->texture = nullptr;
	}
	
	//update next phase so PostUpdate will re-create textures
	coinTossData->phase.next = COIN_TOSS_PHASE_RESULT;
}




