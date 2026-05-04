//Six options on offense and defense
//Off: Run, Short Pass, Long Pass, Kneel, Kick, Punt
//Def: Base, Man, Cover, Prevent, Goaline, Blitz

//Run: Low fumble %, likely to gain low yards
//Short Pass: Standard fumble %, less likely than run but better yardage
//Long Pass: high int %, low likelyhood of success but results in a large play
//Kneel: Lose a yard and increase a down - only fails on two successive 0 rolls (1 in 1000)
//Kick: Attempt a field goal. % changes based on distance to goal
//Punt: Punt the ball - 1 % chance he fumbles the punt - another 3% chance the recovering team fumbles - calc result seperate

//Base:    0RN 0SP 0LP 
//Man:     +RN +SP -LP
//Cover:   -RN +SP +LP
//Prevent: -RN 0SP +LP
//Goal Line  +RN -SP -LP
//Blitz    ?RN -SP ?LP

#include "play_calling.h"

#include <stdlib.h>

#include "colors.h"
#include "match.h"
#include "render.h"
#include "scoreboard.h"
#include "update.h"

static void PlayCalling_Init_UI(GameEngine *eng, GameData *data);

static void PlayCalling_Init_UIStrings(PlayCallingData *data, const MatchPossession pos);

static void PlayCalling_Init_ButtonStrings_Offense(const char *strings[PLAY_CALLING_UI_COUNT]);
static void PlayCalling_Init_ButtonStrings_Defense(const char *strings[PLAY_CALLING_UI_COUNT]);
static void PlayCalling_Init_ButtonStrings_Error(const char *strings[PLAY_CALLING_UI_COUNT]);
	
static void PlayCalling_Init_UIData(PlayCallingData *data);

static void PlayCalling_Init_OnClickFuncs(PlayCallingData *data, const MatchPossession pos);
static void PlayCalling_Init_OnClickFuncs_OffenseButtons(PlayCallingData *data);
static void PlayCalling_Init_OnClickFuncs_DefenseButtons(PlayCallingData *data);

static void PlayCalling_Init_UITextures(GameEngine *eng, PlayCallingData *data);

//Take entire PlayCallingData not just uiData to handle Scoreboard
static void PlayCalling_ResizeLayout(PlayCallingData *data, const Vector2 windowSize, const MatchPossession pos);

static void PlayCalling_CheckButtonHover(UIData *data, const FVector2 mousePos);
static PlayCallingUIElement PlayCalling_CheckButtonClick(UIData *ui, const FVector2 mousePos);

//On Clicks
static void PlayCalling_Run_OnClick(GameData *data);
static void PlayCalling_ShortPass_OnClick(GameData *data);
static void PlayCalling_LongPass_OnClick(GameData *data);
static void PlayCalling_Kneel_OnClick(GameData *data);
static void PlayCalling_Kick_OnClick(GameData *data);
static void PlayCalling_Punt_OnClick(GameData *data);

static void PlayCalling_Base_OnClick(GameData *data);
static void PlayCalling_Man_OnClick(GameData *data);
static void PlayCalling_Cover_OnClick(GameData *data);
static void PlayCalling_Prevent_OnClick(GameData *data);
static void PlayCalling_GoalLine_OnClick(GameData *data);
static void PlayCalling_Blitz_OnClick(GameData *data);

static void PlayCalling_Quit_OnClick(GameData *data);

static void PlayCalling_HandlePlayerSelection(MatchCtx *matchCtx, const PlayID playerPlay);


//INIT
void PlayCalling_Init(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;

	matchCtx->matchStateData = calloc(1, sizeof(PlayCallingData));
	if (matchCtx->matchStateData == nullptr) {
		Error_Alert(data, ERROR_ALLOC, "PlayCallingData failed calloc()");
		return;
	}
	
	PlayCallingData *playCallingData = matchCtx->matchStateData;

	//Non scoreboard stuff
	PlayCalling_Init_UI(eng, data);

	Scoreboard_Init(eng, &playCallingData->scoreboard, data->teamAssignment, matchCtx->session.pos);

	//Initial layout setup et check button hover

	PlayCalling_ResizeLayout(playCallingData, data->window.size, matchCtx->session.pos);
}

//CLEANUP
void PlayCalling_Cleanup(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	PlayCallingData *playCallingData = matchCtx ->matchStateData;

	//Cleanup base textures
	for (s32 i = PLAY_CALLING_UI_START; i < PLAY_CALLING_UI_END; i++) {
		UIData *uiData = &playCallingData->uiData[i];
		if (uiData->texture) {
			SDL_DestroyTexture(uiData->texture);
			uiData->texture = nullptr;
		}
	}

	//Cleanup Scoreboard
	Scoreboard_Cleanup(eng, &playCallingData->scoreboard);
	
	//the sub-state data pointer is freed and cleared by Match_Cleanup_MatchStateData()
}

//UPDATE
void PlayCalling_Update(GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	PlayCallingData *playCallingData = matchCtx->matchStateData;

	if (data->window.resized) {
		//NOTE: play calling requiress full PlayCallingData for scoreboard
		PlayCalling_ResizeLayout(playCallingData, data->window.size, matchCtx->session.pos);
	}
	
	if (data->mouse.moved) {
		PlayCalling_CheckButtonHover(playCallingData->uiData, data->mouse.pos);
	}
	
	if (data->mouse.left.wasPressed) {
		PlayCallingUIElement clicked = PlayCalling_CheckButtonClick(playCallingData->uiData, data->mouse.pos);

		if (clicked != PLAY_CALLING_UI_NONE) {
			UIData dataClicked = playCallingData->uiData[clicked];
			if (dataClicked.onClick) {
				OnClick onClick = dataClicked.onClick;
				onClick(data);
			}
		}
	}

}

//POST UPDATE
void PlayCalling_PostUpdate(GameEngine *eng, MatchCtx *matchCtx)
{
	(void)eng;
	(void)matchCtx;
}

//RENDER
void PlayCalling_Render(const GameEngine *eng, const GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	PlayCallingData *playCallingData = matchCtx->matchStateData;

	Render_ClearWhite(eng->renderer);

	//Standard UI Elements
	for (s32 i = PLAY_CALLING_UI_START; i < PLAY_CALLING_UI_END; i++) {
		UIData *uiData = &playCallingData->uiData[i];
		UI_RenderUIElement(eng, uiData);
	}
	
	//Scoreboard UI Elements
	Scoreboard_Render(eng, playCallingData->scoreboard.uiData);

}

static void PlayCalling_Init_UI(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	PlayCallingData *playCallingData = matchCtx->matchStateData;

	PlayCalling_Init_UIStrings(playCallingData, matchCtx->session.pos);

	PlayCalling_Init_UIData(playCallingData);

	//On Clicks
	PlayCalling_Init_OnClickFuncs(playCallingData, matchCtx->session.pos);
	
	PlayCalling_Init_UITextures(eng, playCallingData);
}

static void PlayCalling_Init_UIStrings(PlayCallingData *data, const MatchPossession pos)
{
	//Play Calling buttons
	if (pos == POSSESSION_PLAYER) {
		PlayCalling_Init_ButtonStrings_Offense(data->uiStrings);
	} else if (pos == POSSESSION_CPU) {
		PlayCalling_Init_ButtonStrings_Defense(data->uiStrings);
	} else {
		//TODO: Make this whole func return an error code to call the error alert system or really overhaul the entire alert system eventually.
		PlayCalling_Init_ButtonStrings_Error(data->uiStrings);
	}

	//Quit Button
	data->uiStrings[PLAY_CALLING_UI_QUIT] = "QUIT";
}

static void PlayCalling_Init_ButtonStrings_Offense(const char *strings[PLAY_CALLING_UI_COUNT])
{
	strings[PLAY_CALLING_UI_BUTTON1] = "RUN";
	strings[PLAY_CALLING_UI_BUTTON2] = "SHORT PASS";
	strings[PLAY_CALLING_UI_BUTTON3] = "LONG PASS";
	strings[PLAY_CALLING_UI_BUTTON4] = "KNEEL";
	strings[PLAY_CALLING_UI_BUTTON5] = "KICK";
	strings[PLAY_CALLING_UI_BUTTON6] = "PUNT";
}

static void PlayCalling_Init_ButtonStrings_Defense(const char *strings[PLAY_CALLING_UI_COUNT])
{
	strings[PLAY_CALLING_UI_BUTTON1] = "BASE";
	strings[PLAY_CALLING_UI_BUTTON2] = "MAN";
	strings[PLAY_CALLING_UI_BUTTON3] = "COVER";
	strings[PLAY_CALLING_UI_BUTTON4] = "PREVENT";
	strings[PLAY_CALLING_UI_BUTTON5] = "GOALINE";
	strings[PLAY_CALLING_UI_BUTTON6] = "BLITZ";
}

static void PlayCalling_Init_ButtonStrings_Error(const char *strings[PLAY_CALLING_UI_COUNT])
{
	for (s32 i = PLAY_CALLING_PLAY_BUTTONS_START; i < PLAY_CALLING_PLAY_BUTTONS_END; i++) {
		strings[i] = "NULL";
	}
}
static void PlayCalling_Init_UIData(PlayCallingData *data)
{
	UIData *ui = nullptr;

	//Scoreboard
	ui = &data->uiData[PLAY_CALLING_UI_SCOREBOARD];
	ui->type = UI_TYPE_INFO_BOX;
	ui->bg = COLOR_WHITE;
	ui->hasBackground = true;
	ui->outlineColor = COLOR_BLACK;
	ui->outlined = true;

	//Scoreboard Members

	//init play buttons - all the same
	for (s32 i = PLAY_CALLING_PLAY_BUTTONS_START; i < PLAY_CALLING_PLAY_BUTTONS_END; i++) {
		ui = &data->uiData[i];
		ui->type = UI_TYPE_BUTTON_CONTRAST;
		ui->fg = COLOR_BLACK;
		ui->bg = COLOR_WHITE;
		ui->hasBackground = true;
		ui->outlineColor = COLOR_BLACK;
		ui->outlined = true;
	}

	//Quit Button
	ui = &data->uiData[PLAY_CALLING_UI_QUIT];
	UI_SetupBackButton(ui);
}

static void PlayCalling_Init_OnClickFuncs(PlayCallingData *data, const MatchPossession pos)
{
	if (pos == POSSESSION_PLAYER) {
		PlayCalling_Init_OnClickFuncs_OffenseButtons(data);
	} else if (pos == POSSESSION_CPU) {
		PlayCalling_Init_OnClickFuncs_DefenseButtons(data);
	} else {
		//TODO: Make this whole func return an error code to call the error alert system or really overhaul the entire alert system eventually.
//		PlayCalling_Init_OnClick_Error(data->uiStrings);
		SDL_Log("PlayCalling Play Buttons OnClick did not init!");
	}

	UIData *ui = &data->uiData[PLAY_CALLING_UI_QUIT];
	ui->onClick = PlayCalling_Quit_OnClick;
}

static void PlayCalling_Init_OnClickFuncs_OffenseButtons(PlayCallingData *data)
{
	UIData *ui = nullptr;

	//1 Run
	ui = &data->uiData[PLAY_CALLING_UI_BUTTON1];
	ui->onClick = PlayCalling_Run_OnClick;

	//2 Short Pass
	ui = &data->uiData[PLAY_CALLING_UI_BUTTON2];
	ui->onClick = PlayCalling_ShortPass_OnClick;

	//3 Long Pass
	ui = &data->uiData[PLAY_CALLING_UI_BUTTON3];
	ui->onClick = PlayCalling_LongPass_OnClick;

	//4 Kneel
	ui = &data->uiData[PLAY_CALLING_UI_BUTTON4];
	ui->onClick = PlayCalling_Kneel_OnClick;

	//5 Kick
	ui = &data->uiData[PLAY_CALLING_UI_BUTTON5];
	ui->onClick = PlayCalling_Kick_OnClick;

	//6 Punt
	ui = &data->uiData[PLAY_CALLING_UI_BUTTON6];
	ui->onClick = PlayCalling_Punt_OnClick;
}

static void PlayCalling_Init_OnClickFuncs_DefenseButtons(PlayCallingData *data)
{
	UIData *ui = nullptr;

	//1 Base
	ui = &data->uiData[PLAY_CALLING_UI_BUTTON1];
	ui->onClick = PlayCalling_Base_OnClick;

	//2 Man 
	ui = &data->uiData[PLAY_CALLING_UI_BUTTON2];
	ui->onClick = PlayCalling_Man_OnClick;

	//3 Cover 
	ui = &data->uiData[PLAY_CALLING_UI_BUTTON3];
	ui->onClick = PlayCalling_Cover_OnClick;

	//4 Prevent
	ui = &data->uiData[PLAY_CALLING_UI_BUTTON4];
	ui->onClick = PlayCalling_Prevent_OnClick;

	//5 Goal Line
	ui = &data->uiData[PLAY_CALLING_UI_BUTTON5];
	ui->onClick = PlayCalling_GoalLine_OnClick;

	//6 Blitz
	ui = &data->uiData[PLAY_CALLING_UI_BUTTON6];
	ui->onClick = PlayCalling_Blitz_OnClick;
}

static void PlayCalling_Init_UITextures(GameEngine *eng, PlayCallingData *data)
{
	//Base UI
	for (s32 i = PLAY_CALLING_UI_START; i < PLAY_CALLING_UI_END; i++) {
		UIData *ui = &data->uiData[i];
		ui->texture = Text_CreateUITexture(eng, data->uiStrings[i], ui);
	}

	//Scoreboard Members
}

static void PlayCalling_ResizeLayout(PlayCallingData *data, const Vector2 windowSize, const MatchPossession pos)
{
	//Local UIData variable - this func needs full PlayCallingData to handle scoreboard as well
	UIData *ui = data->uiData;

	f32 wX = (f32)windowSize.x;
	f32 wY = (f32)windowSize.y;

	SDL_FRect *dest = nullptr;

	//Scoreboard 
	dest = &ui[PLAY_CALLING_UI_SCOREBOARD].dest;

	dest->w = wX * 0.8f;
	dest->h = wY * 0.2f;
	dest->x = (wX - dest->w) * 0.5f;
	dest->y = wY * 0.1f;

	Scoreboard_ResizeLayout(*dest, &data->scoreboard, pos);
	
	//Play Buttons
	SDL_FRect buttonArea = {};
	
	buttonArea.w = wX * 0.8f;
	buttonArea.h = wY * 0.2f;
	buttonArea.x = (wX - dest->w) * 0.5f;
	buttonArea.y = wY * 0.4f;

	s32 numButtonsInRow = 3;
	s32 numSpaces = numButtonsInRow - 1;
	f32 spacesW = (buttonArea.w * 0.2f) / (f32)numSpaces;

	//Top 3
	buttonArea.y -= wY * 0.05f;
	for (s32 i = PLAY_CALLING_PLAY_BUTTONS_ROW1_START; i < PLAY_CALLING_PLAY_BUTTONS_ROW1_END; i++) {
	
		dest = &ui[i].dest;

		dest->h = buttonArea.h * 0.5f;
		dest->y = buttonArea.y;
		dest->w = (buttonArea.w * 0.8f) / (f32)numButtonsInRow;
		if (i == PLAY_CALLING_PLAY_BUTTONS_ROW1_START) {
			dest->x = buttonArea.x;
		} else {
			dest->x = ui[i - 1].dest.x + dest->w + spacesW;
		}
	}
	
	//Bottom 3
	buttonArea.y += wY * 0.15f;

	for (s32 i = PLAY_CALLING_PLAY_BUTTONS_ROW2_START; i < PLAY_CALLING_PLAY_BUTTONS_ROW2_END; i++) {

		dest = &ui[i].dest;

		dest->h = buttonArea.h * 0.5f;
		dest->y = buttonArea.y;
		dest->w = (buttonArea.w * 0.8f) / (f32)numButtonsInRow;
		if (i == PLAY_CALLING_PLAY_BUTTONS_ROW2_START) {
			dest->x = buttonArea.x;
		} else {
			dest->x = ui[i - 1].dest.x + dest->w + spacesW;
		}
	}

	//Quit Button
	dest = &ui[PLAY_CALLING_UI_QUIT].dest;
	*dest = UI_GetBackButtonDestRect(wX, wY);
}

static void PlayCalling_CheckButtonHover(UIData *data, const FVector2 mousePos)
{
	for (s32 i = PLAY_CALLING_BUTTONS_START; i < PLAY_CALLING_BUTTONS_END; i++) {
		UIData *ui = &data[i];
		
		if (data->hidden) {
			continue;
		}
		UI_UpdateHover(ui, mousePos);
	}
}

static PlayCallingUIElement PlayCalling_CheckButtonClick(UIData *ui, const FVector2 mousePos) 
{
	for (s32 i = PLAY_CALLING_BUTTONS_START; i < PLAY_CALLING_BUTTONS_END; i++) {
		 if (UI_CheckClick(&ui[i], mousePos)) {
			 return i;
		 }
	}

	return PLAY_CALLING_UI_NONE;
}

//ON CLICK
static void PlayCalling_Run_OnClick(GameData *data)
{
	(void)data;
}

static void PlayCalling_ShortPass_OnClick(GameData *data)
{
	(void)data;
}

static void PlayCalling_LongPass_OnClick(GameData *data)
{
	(void)data;
}

static void PlayCalling_Kneel_OnClick(GameData *data)
{
	(void)data;
}

static void PlayCalling_Kick_OnClick(GameData *data)
{
	(void)data;
}

static void PlayCalling_Punt_OnClick(GameData *data)
{
	(void)data;
}


static void PlayCalling_Base_OnClick(GameData *data)
{
	(void)data;
}

static void PlayCalling_Man_OnClick(GameData *data)
{
	(void)data;
}

static void PlayCalling_Cover_OnClick(GameData *data)
{
	(void)data;
}

static void PlayCalling_Prevent_OnClick(GameData *data)
{
	(void)data;
}

static void PlayCalling_GoalLine_OnClick(GameData *data)
{
	(void)data;
}

static void PlayCalling_Blitz_OnClick(GameData *data)
{
	(void)data;
}


static void PlayCalling_Quit_OnClick(GameData *data)
{
	RequestGameStateTransition(data, MAIN_STATE_MAIN_MENU);
}


static void PlayCalling_HandlePlayerSelection(MatchCtx *matchCtx, const PlayID playerPlay)
{
	//Input - Determine offense and defense plays
	PlayID offPlay, defPlay;

	if (PlayCalling_IsOffensePlay(playerPlay)) {
			offPlay = playerPlay;
			defPlay = PlayCalling_GetCPUPlay_Def(matchCtx);
	} else {
		defPlay = playerPlay;
		offPlay = PlayCalling_GetCPUPlay_Off(matchCtx);
	}
			
	//Sim - Feed plays to Sim, retrun PlayResult

	//Update - apply PlayResult to scoreboard et match session
	
	//Check game over

	//Check change possession

	//Destroy stale textures

	//Update Textures Flag

}


