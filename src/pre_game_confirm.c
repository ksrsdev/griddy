#include "pre_game_confirm.h"

#include <stdlib.h>

#include "colors.h"
#include "init.h"
#include "render.h"
#include "state_data.h"

static void PreGameConfirm_LoadUIStrings(const GameData *data);
static void PreGameConfirm_InitUIData(const GameEngine *eng, const GameData *data);
static void PreGameConfirm_SetupUI(PreGameConfirmData *data, TeamID playerID, TeamID cpuID);
static void PreGameConfirm_ResizeLayout(UIData *data, const Vector2 windowSize);

static void PreGameConfirm_CreateTextures(const GameEngine *eng, PreGameConfirmData *data);

static void PreGameConfirm_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos);

static void PreGameConfirm_ResolveRandomTeam(TeamAssignment *teamAssignment);

//   ###   INIT   ###
void PreGameConfirm_Init(GameEngine *eng, GameData *data)
{
	data->stateData = calloc(1, sizeof(PreGameConfirmData));
	if (data->stateData == nullptr) {
		//error.c errors are fatal
		Error_Alert(data, ERROR_ALLOC, "opionsMenuData failed calloc()");
		return;
	}

	//Resolve random teams
	TeamAssignment *teamAssignment = &data->teamAssignment;

	if (teamAssignment->player == TEAM_ID_RANDOM || teamAssignment->cpu == TEAM_ID_RANDOM) {
		PreGameConfirm_ResolveRandomTeam(teamAssignment);
	}

	PreGameConfirm_LoadUIStrings(data);
	PreGameConfirm_InitUIData(eng, data);

}

void PreGameConfirm_Cleanup(GameEngine *eng, GameData *data)
{
	(void)eng;

	PreGameConfirmData *preGameConfirmData = data->stateData;

	for (s32 i = PRE_GAME_CONFIRM_UI_START; i < PRE_GAME_CONFIRM_UI_END; i++) {

		UIData *uiData = &preGameConfirmData->uiData[i];
		if (uiData->texture) {
			SDL_DestroyTexture(uiData->texture);
			uiData->texture = nullptr;
		}
	}

	Deinit_StateData(&data->stateData);
}

//   ###   UPDATE   ###
void PreGameConfirm_Update(GameData *data)
{
	PreGameConfirmData *preGameConfirmData = data->stateData;

	if (data->window.resized) {
		PreGameConfirm_ResizeLayout(preGameConfirmData->uiData, data->window.size);
	}
	
	if (data->mouse.moved) {
		PreGameConfirm_CheckButtonHighlight(preGameConfirmData->uiData, data->mouse.pos);
	}

}

//   ###   RENDER   ###
void PreGameConfirm_Render(const GameEngine *eng, const GameData *data)
{
	PreGameConfirmData *preGameConfirmData = data->stateData;

	//Clear White
	Render_SetDrawColor(eng->renderer, COLOR_WHITE);
	SDL_RenderClear(eng->renderer);

	for (s32 i = PRE_GAME_CONFIRM_UI_START; i < PRE_GAME_CONFIRM_UI_END; i++) {
		UIData *uiData = &preGameConfirmData->uiData[i];
		UI_RenderUIElement(eng, uiData);
	}

}

static void PreGameConfirm_LoadUIStrings(const GameData *data)
{
	PreGameConfirmData *preGameConfirm = data->stateData;

	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_TITLE] = "CONFIRM";
	
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_PLAYER_TITLE] = "PLAYER";
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_CPU_TITLE] = "CPU";
	
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_VS] = "VS.";

	TeamID playerTeamID = data->teamAssignment.player;
	TeamID cpuTeamID = data->teamAssignment.cpu;

	TeamDescription playerTeamDesc = gTeamDescriptions[playerTeamID];
	TeamDescription cpuTeamDesc = gTeamDescriptions[cpuTeamID];
	
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_PLAYER_BOX] = playerTeamDesc.title;
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_CPU_BOX] = cpuTeamDesc.title;
	
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_PLAYER_PREVIEW] = "PREVIEW";
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_CPU_PREVIEW] = "PREVIEW";
	
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_BACK] = "<-BACK";
	preGameConfirm->uiStrings[PRE_GAME_CONFIRM_UI_PLAY] = "PLAY GAME";
}

static void PreGameConfirm_InitUIData(const GameEngine *eng, const GameData *data)
{
	PreGameConfirmData *preGameConfirmData = data->stateData;
	TeamID playerID = data->teamAssignment.player;
	TeamID cpuID = data->teamAssignment.cpu;

	PreGameConfirm_SetupUI(preGameConfirmData, playerID, cpuID);

	PreGameConfirm_ResizeLayout(preGameConfirmData->uiData, data->window.size);

	PreGameConfirm_CheckButtonHighlight(preGameConfirmData->uiData, data->mouse.pos);

	PreGameConfirm_CreateTextures(eng, preGameConfirmData);
}

static void PreGameConfirm_SetupUI(PreGameConfirmData *data, TeamID playerID, TeamID cpuID)
{
	UIData *uiData = nullptr;
	TeamDescription playerDesc = gTeamDescriptions[playerID];
	TeamDescription cpuDesc = gTeamDescriptions[cpuID];
	
	//Title
	uiData = &data->uiData[PRE_GAME_CONFIRM_UI_TITLE];
	uiData->type = UI_TYPE_TEXT;
	uiData->fg = COLOR_BLACK;
	
	//Player Title
	uiData = &data->uiData[PRE_GAME_CONFIRM_UI_PLAYER_TITLE];
	uiData->type = UI_TYPE_TEXT;
	uiData->fg = COLOR_BLACK;

	//CPU Title
	uiData = &data->uiData[PRE_GAME_CONFIRM_UI_CPU_TITLE];
	uiData->type = UI_TYPE_TEXT;
	uiData->fg = COLOR_BLACK;

	//VS
	uiData = &data->uiData[PRE_GAME_CONFIRM_UI_VS];
	uiData->type = UI_TYPE_TEXT;
	uiData->fg = COLOR_BLACK;

	//Player Box
	uiData = &data->uiData[PRE_GAME_CONFIRM_UI_PLAYER_BOX];
	uiData->type = UI_TYPE_TEXT;
	
	uiData->bg = playerDesc.color;

	if (!Colors_AreEqual(uiData->bg, COLOR_BLACK)) {
		uiData->fg = COLOR_BLACK;
	} else {
		uiData->fg = COLOR_WHITE;
	}

	//CPU Box
	uiData = &data->uiData[PRE_GAME_CONFIRM_UI_CPU_BOX];
	uiData->type = UI_TYPE_TEXT;
	
	uiData->bg = cpuDesc.color;

	if (!Colors_AreEqual(uiData->bg, COLOR_BLACK)) {
		uiData->fg = COLOR_BLACK;
	} else {
		uiData->fg = COLOR_WHITE;
	}

	//Player Preview
	uiData = &data->uiData[PRE_GAME_CONFIRM_UI_PLAYER_PREVIEW];
	UI_SetupDefaultButton(uiData);

	//CPU Preview
	uiData = &data->uiData[PRE_GAME_CONFIRM_UI_CPU_PREVIEW];
	UI_SetupDefaultButton(uiData);

	//Back
	uiData = &data->uiData[PRE_GAME_CONFIRM_UI_BACK];
	UI_SetupBackButton(uiData);

	//Play
	uiData = &data->uiData[PRE_GAME_CONFIRM_UI_PLAY];
	UI_SetupButton(uiData, COLOR_BLACK, COLOR_GREEN);
}

static void PreGameConfirm_ResizeLayout(UIData *data, const Vector2 windowSize)
{
	f32 wX = (f32)windowSize.x;
	f32 wY = (f32)windowSize.y;

	SDL_FRect *dest = nullptr;

	//Title
	dest = &data[PRE_GAME_CONFIRM_UI_TITLE].destRect;
	*dest = UI_GetTitleDestRect(wX, wY);

	//Player Title
	dest = &data[PRE_GAME_CONFIRM_UI_PLAYER_TITLE].destRect;

	dest->w = wX * 0.25f;
	dest->h = wY * 0.1f;
	dest->x = wX / 8.0f;
	dest->y = wY * 0.4f;
	
	//CPU Title
	dest = &data[PRE_GAME_CONFIRM_UI_CPU_TITLE].destRect;
	
	dest->w = wX * 0.25f;
	dest->h = wY * 0.1f;
	dest->x = wX - (dest->w) - (wX / 8.0f);
	dest->y = wY * 0.4f;

	//VS
	dest = &data[PRE_GAME_CONFIRM_UI_VS].destRect;

	dest->w = wX * 0.25f;
	dest->h = wY * 0.2f;
	dest->x = (wX * 0.5f) - (dest->w * 0.5f);
	dest->y = wY * 0.5f;

	//Player Box
	dest = &data[PRE_GAME_CONFIRM_UI_PLAYER_BOX].destRect;

	dest->w = wX * 0.25f;
	dest->h = wY * 0.2f;
	dest->x = wX / 8.0f;
	dest->y = wY * 0.5f;

	//CPU Box
	dest = &data[PRE_GAME_CONFIRM_UI_CPU_BOX].destRect;
	
	dest->w = wX * 0.25f;
	dest->h = wY * 0.2f;
	dest->x = wX - (dest->w) - (wX / 8.0f);
	dest->y = wY * 0.5f;

	//Player Preview
	dest = &data[PRE_GAME_CONFIRM_UI_PLAYER_PREVIEW].destRect;

	dest->w = wX * 0.25f;
	dest->h = wY * 0.1f;
	dest->x = wX / 8.0f;
	dest->y = wY * 0.75f;

	//CPU Preview
	dest = &data[PRE_GAME_CONFIRM_UI_CPU_BOX].destRect;
	
	dest->w = wX * 0.25f;
	dest->h = wY * 0.1f;
	dest->x = wX - (dest->w) - (wX / 8.0f);
	dest->y = wY * 0.75f;

	//Back
	dest = &data[PRE_GAME_CONFIRM_UI_BACK].destRect;

	*dest = UI_GetBackButtonDestRect(wX, wY);

	//Play
	dest = &data[PRE_GAME_CONFIRM_UI_PLAY].destRect;

	dest->w = wX * 0.25f;
	dest->h = wY * 0.1f;
	dest->x = (wX * 0.5f) - (dest->w * 0.5f);
	dest->y = wY * 0.8f;
}

static void PreGameConfirm_CreateTextures(const GameEngine *eng, PreGameConfirmData *data)
{
	for (s32 i = PRE_GAME_CONFIRM_UI_START; i < PRE_GAME_CONFIRM_UI_END; i++) {

		UIData *uiData = &data->uiData[i];

		uiData->texture = Text_CreateUITexture(eng, data->uiStrings[i], uiData);
	}
}

static void PreGameConfirm_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos)
{
	for (s32 i = PRE_GAME_CONFIRM_UI_BUTTON_START; i < PRE_GAME_CONFIRM_UI_BUTTON_END; i++) {
		
		UIData *data = &uiData[i];
		if (data->hidden) {
			continue;
		}
		UI_UpdateHover(data, mousePos);
	}
}

static void PreGameConfirm_ResolveRandomTeam(TeamAssignment *teamAssignment)
{
	TeamID randomID = (rand() % ((TEAM_ID_COUNT - 1) - (TEAM_ID_RANDOM + 1) + 1)) + (TEAM_ID_RANDOM + 1);

	if (teamAssignment->player == TEAM_ID_RANDOM) {
		
		while (randomID == teamAssignment->cpu) {
			randomID = (rand() % ((TEAM_ID_COUNT - 1) - (TEAM_ID_RANDOM + 1) + 1)) + (TEAM_ID_RANDOM + 1);
		}
		teamAssignment->player = randomID;
	}
	
	if (teamAssignment->cpu == TEAM_ID_RANDOM) {
		
		while (randomID == teamAssignment->player) {
			randomID = (rand() % ((TEAM_ID_COUNT - 1) - (TEAM_ID_RANDOM + 1) + 1)) + (TEAM_ID_RANDOM + 1);
		}
		teamAssignment->cpu = randomID;
	}
}
