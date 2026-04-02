#include "team_select.h"

#include <stdlib.h>
#include <stdio.h>

#include "colors.h"
#include "context.h"
#include "init.h"
#include "render.h"
#include "state_data.h"
#include "update.h"

//String init
static void TeamSelect_LoadUIStrings(const GameData *data);
const char * TeamSelect_GetTitleText(const TeamAssignment *assignment);

//Layout etc init
static void TeamSelect_LoadUIData(const GameEngine *eng, const GameData *data);
static void TeamSelect_InitUIData(TeamSelectData *data);
static void TeamSelect_AssignOnClickFuncs(TeamSelectData *data);

//Resize event
static void TeamSelect_ResizeLayout(UIData *data, const Vector2 windowSize);
static void TeamSelect_ResizeInfoBoxMembers(UIData *data);

//Textures
static void TeamSelect_CreateTextures(const GameEngine *eng, TeamSelectData *data);
static void TeamSelect_UpdateInfoBoxTextures(const GameEngine *eng, TeamSelectData *data, const TeamID id);
static void TeamSelect_UpdateInfoBoxMembersTextures(const GameEngine *eng, TeamSelectData *data);
static void TeamSelect_UpdateTitleTexture(const GameEngine *eng, TeamSelectData *data);

//Mouse Utility
static void TeamSelect_CheckButtonHighlight(UIData *uiDat, const FVector2 mousePos);
static TeamSelectUIElement TeamSelect_CheckButtonClick(UIData *uiData, const FVector2 mousePos);

//Team Select Buttons
static void TeamSelect_UpdateFocusTeam(GameData *data, TeamID id);
static void TeamSelect_LoadRandomInfoBox(TeamSelectData *data);
static void TeamSelect_LoadTeamInfoBox(TeamSelectData *data, TeamID id);
static void TeamSelect_ResetPlayerTeamSelection(TeamSelectData *data);
static void TeamSelect_SetupCPUTeamSelection(TeamSelectData *data);

static void TeamSelect_RandomButton_OnClick(GameData *data);
static void TeamSelect_BlackButton_OnClick(GameData *data);
static void TeamSelect_WhiteButton_OnClick(GameData *data);
static void TeamSelect_GreenButton_OnClick(GameData *data);
static void TeamSelect_RedButton_OnClick(GameData *data);
static void TeamSelect_PinkButton_OnClick(GameData *data);
static void TeamSelect_BrownButton_OnClick(GameData *data);
static void TeamSelect_YellowButton_OnClick(GameData *data);
static void TeamSelect_OrangeButton_OnClick(GameData *data);
static void TeamSelect_BlueButton_OnClick(GameData *data);

//Navigation Buttons
static void TeamSelect_BackButton_OnClick(GameData *data);
//static void TeamSelect_PreviewButton_OnClick(GameData *data);
static void TeamSelect_ContinueButton_OnClick(GameData *data);

//Misc helper stuff
static void TeamSelect_UpdateRainbowColor(UIData *randomButton, u64 *hueBaseTime);

static const SDL_Color sTeamButtonColors[TEAM_SELECT_UI_TEAM_BUTTON_ROW_2_END - TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_START] = {
	COLOR_BLACK,
	COLOR_WHITE,
	COLOR_GREEN,
	COLOR_RED,
	COLOR_PINK,
	COLOR_BROWN,
	COLOR_YELLOW,
	COLOR_ORANGE,
	COLOR_BLUE
};

typedef struct {
	const char *title;
	const char *desc;
	const char *pros;
	const char *cons;
	const char *off;
	const char *def;
} TeamDescription;

static const TeamDescription sTeamDescriptions[TEAM_ID_COUNT] = {
	[TEAM_ID_NONE] = {
		.title = "NONE",
		.desc  = "NONE",
		.pros  = "NONE",
		.cons  = "NONE",
		.off   = "NONE",
		.def   = "NONE"
	},
	[TEAM_ID_RANDOM] = {
		.title = "NONE",
		.desc  = "NONE",
		.pros  = "NONE",
		.cons  = "NONE",
		.off   = "NONE",
		.def   = "NONE"
	},
	[TEAM_ID_BLACK] = {
		.title = "BLACK",
		.desc = "LESS THINKING, MORE HITTING.",
		.pros = "RB, WR, DL, LB, DB",
		.cons = "OL, QB, TE, K",
		.off  = "11 PISTOL",
		.def  = "3-4",
	},
	[TEAM_ID_WHITE] = {
		.title = "WHITE",
		.desc  = "A GOOD PLAN BEATS BRUTE FORCE ANYDAY.",
		.pros  = "OL, QB, TE, K",
		.cons  = "RB, WR, DL, LB, DB",
		.off   = "11 GUN BUNCH Y FLEX",
		.def   = "4-3",
	},
	[TEAM_ID_GREEN] = {
		.title = "GREEN",
		.desc  = "SCORE 50 POINTS OR GET SACKED TRYING.",
		.pros  = "QB, WR, LB",
		.cons  = "OL, DB, K",
		.off   = "10 SHOTGUN SPREAD",
		.def   = "3-4",
	},
	[TEAM_ID_RED] = {
		.title = "RED",
		.desc  = "FIRST WE SCORE, THEN WE SCORE SOME MORE.",
		.pros  = "ALL OFFENSIVE POSITIONS",
		.cons  = "ALL DEFENSIVE POSITIONS",
		.off   = "21 PRO SET",
		.def   = "4-2",
	},
	[TEAM_ID_PINK] = {
		.title = "PINK",
		.desc  = "WHO NEEDS TO HIT HARD WHEN YOU CAN OUTRUN THE COMPETION?",
		.pros  = "WR, DB",
		.cons  = "OL, DL",
		.off   = "00 GUN EMPTY BUNCH TIGHT",
		.def   = "3-2-6",
	},
	[TEAM_ID_BROWN] = {
		.title = "BROWN",
		.desc  = "THIS AIN'T YOUR GRANPA'S FOOTBALL TEAM, IT'S YOUR GREAT-GRANDPA'S.",
		.pros  = "OL, DL",
		.cons  = "QB, DB",
		.off   = "32 T FORMATION",
		.def   = "6-2-3",
	},
	[TEAM_ID_YELLOW] = {
		.title = "YELLOW",
		.desc  = "BIG BODIES BEAT SMALL BODIES EVERYTIME.",
		.pros  = "OL, DL, TE",
		.cons  = "WR, DB",
		.off   = "12 ACE H-BACK",
		.def   = "5-2-4",
	},
	[TEAM_ID_ORANGE] = {
		.title = "ORANGE",
		.desc  = "GOOD OLD WISHBONE, NOTHING BEATS THAT!",
		.pros  = "QB,FB,HB",
		.cons  = "WR,DL,K",
		.off   = "31 WISHBONE",
		.def   = "4-4",
	},
	[TEAM_ID_BLUE] = {
		.title = "BLUE",
		.desc  = "YOU CAN'T LOSE IF YOUR OPPONENT CAN'T SCORE.",
		.pros  = "ALL DEFENSIVE POSITIONS.",
		.cons  = "ALL OFFENSIVE POSITIONS.",
		.off   = "21 I PRO",
		.def   = "4-3",
	},
};

//   ###   INIT   ###
void TeamSelect_Init(GameEngine *eng, GameData *data)
{
	data->stateData = calloc(1, sizeof(TeamSelectData));
	if (data->stateData == nullptr) {
		//error.c errors are fatal
		Error_Alert(data, ERROR_ALLOC, "teamSelectData failed calloc()");
		return;
	}
	
	TeamSelect_LoadUIStrings(data);

	TeamSelect_LoadUIData(eng, data);
}

void TeamSelect_Cleanup(GameEngine *eng, GameData *data)
{
	(void)eng;

	TeamSelectData *teamSelectData = data->stateData;

	for (s32 i = TEAM_SELECT_UI_START; i < TEAM_SELECT_UI_END; i++) {

		UIData *uiData = &teamSelectData->uiData[i];
		if (uiData->texture) {
			SDL_DestroyTexture(uiData->texture);
			uiData->texture = nullptr;
		}
	}

	Deinit_StateData(&data->stateData);
}


//   ###   UPDATE   ###
void TeamSelect_Update(GameData *data) 
{
	TeamSelectData *teamSelectData = data->stateData;

	//Update Random Color
	TeamSelect_UpdateRainbowColor(&teamSelectData->uiData[TEAM_SELECT_UI_RANDOM], &teamSelectData->hueBaseTime);

	//If random is focus team update the info box text color
	if (data->teamAssignment.focus == TEAM_ID_RANDOM) {
		teamSelectData->uiData[TEAM_SELECT_UI_INFO_BOX].fg = teamSelectData->uiData[TEAM_SELECT_UI_RANDOM].bg;
	}

	if (data->window.resized) {
		TeamSelect_ResizeLayout(teamSelectData->uiData, data->window.size);
	}
	
	if (data->mouse.moved) {
		TeamSelect_CheckButtonHighlight(teamSelectData->uiData, data->mouse.pos);
	}
	
	if (data->mouse.left.wasPressed) {
		TeamSelectUIElement clicked = TeamSelect_CheckButtonClick(teamSelectData->uiData, data->mouse.pos);

		if (clicked != TEAM_SELECT_UI_NONE) {
			UIData dataClicked = teamSelectData->uiData[clicked];
			if (dataClicked.onClick) {
				OnClick onClick = dataClicked.onClick;
				onClick(data);
			}
		}
	}
}

//   ###   RENDER   ###
void TeamSelect_Render(const GameEngine *eng, const GameData *data)
{
	TeamSelectData *teamSelectData = data->stateData;

	//TODO TeamSelect_CheckUpdateTextures()
	if (teamSelectData->updateInfoBox) {
		TeamSelect_UpdateInfoBoxTextures(eng, teamSelectData, data->teamAssignment.focus);
	}

	if (teamSelectData->updateTitle) {
		TeamSelect_UpdateTitleTexture(eng, teamSelectData);
	}
	
	//Clear White
	Render_SetDrawColor(eng->renderer, COLOR_WHITE);
	SDL_RenderClear(eng->renderer);

	//UI Elements
	for (s32 i = TEAM_SELECT_UI_START; i < TEAM_SELECT_UI_END; i++) {
		UIData *uiData = &teamSelectData->uiData[i];
		UI_RenderUIElement(eng, uiData);
	}
}

//   ###   STRINGS   ###

static void TeamSelect_LoadUIStrings(const GameData *data)
{
	TeamSelectData *teamSelectData = data->stateData;

	//Title
	const char *titleText = TeamSelect_GetTitleText(&data->teamAssignment);
	teamSelectData->uiStrings[TEAM_SELECT_UI_TITLE] = titleText;

	//Info Box
	teamSelectData->uiStrings[TEAM_SELECT_UI_INFO_BOX] = "SELECT A TEAM";

	//Buttons
	teamSelectData->uiStrings[TEAM_SELECT_UI_RANDOM] = "RANDOM";
	teamSelectData->uiStrings[TEAM_SELECT_UI_BLACK]  = "BLACK";
	teamSelectData->uiStrings[TEAM_SELECT_UI_WHITE]  = "WHITE";
	teamSelectData->uiStrings[TEAM_SELECT_UI_GREEN]  = "GREEN";
	teamSelectData->uiStrings[TEAM_SELECT_UI_RED]    = "RED";
	teamSelectData->uiStrings[TEAM_SELECT_UI_PINK]   = "PINK";
	teamSelectData->uiStrings[TEAM_SELECT_UI_BROWN]  = "BROWN";
	teamSelectData->uiStrings[TEAM_SELECT_UI_YELLOW] = "YELLOW";
	teamSelectData->uiStrings[TEAM_SELECT_UI_ORANGE] = "ORANGE";
	teamSelectData->uiStrings[TEAM_SELECT_UI_BLUE]   = "BLUE";
	
	teamSelectData->uiStrings[TEAM_SELECT_UI_BACK]     = "<-BACK";
	teamSelectData->uiStrings[TEAM_SELECT_UI_PREVIEW]  = "VIEW ROSTER";
	teamSelectData->uiStrings[TEAM_SELECT_UI_CONTINUE] = "CONTINUE->";
}

const char * TeamSelect_GetTitleText(const TeamAssignment *assignment)
{
	if (assignment->player == TEAM_ID_NONE) {
		return "SELECT PLAYER TEAM";
	} else if (assignment->cpu == TEAM_ID_NONE) {
		return "SELECT CPU TEAM";
	} else {
		return "TEAM SELECT";
	}
}

//   ###   UIDATA   ###

static void TeamSelect_LoadUIData(const GameEngine *eng, const GameData *data)
{
	TeamSelectData *teamSelectData = data->stateData;

	TeamSelect_InitUIData(teamSelectData);

	TeamSelect_AssignOnClickFuncs(teamSelectData);
	
	TeamSelect_ResizeLayout(teamSelectData->uiData, data->window.size);

	TeamSelect_CheckButtonHighlight(teamSelectData->uiData, data->mouse.pos);
	
	TeamSelect_CreateTextures(eng, teamSelectData);

	//Set base time for hue cycle
	teamSelectData->hueBaseTime = SDL_GetTicks();
	
}

//Just set the stuff that will always be true (type mostly)- this should only run via INIT
static void TeamSelect_InitUIData(TeamSelectData *data)
{
	UIData *uiData = nullptr;

	//Title
	uiData = &data->uiData[TEAM_SELECT_UI_TITLE];

	uiData->type = UI_TYPE_TEXT;
	uiData->fg   = COLOR_BLACK;

	//Mass init all team buttons
	for (s32 i = TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_START; i < TEAM_SELECT_UI_TEAM_BUTTON_ROW_2_END; i++) {

		//Figure out our index for the team colors - remember to offset by 1 for random!
		s32 j = i - TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_START - 1;
		
		uiData = &data->uiData[i];

		uiData->type = UI_TYPE_BUTTON;
		uiData->hasBackground = true;

		//Random button - remember this is just the init. It starts red anyways...
		if (j < 0) {
			uiData->fg = COLOR_BLACK;
			uiData->bg = COLOR_RED;
			uiData->outlineColor = COLOR_BLACK;
		} else { //ALl other buttons get bg from index
			uiData->bg = sTeamButtonColors[j];
			if (Colors_AreEqual(sTeamButtonColors[j], COLOR_BLACK)) { //white fg & red highlight
				uiData->fg = COLOR_WHITE;
				uiData->outlineColor = COLOR_RED;
			} else { //All other buttons follow pattern
				uiData->fg = COLOR_BLACK;
				uiData->outlineColor = COLOR_BLACK;
			}
		}
	}

	//Info Box
	uiData = &data->uiData[TEAM_SELECT_UI_INFO_BOX];

	uiData->type          = UI_TYPE_TEXT;
	uiData->fg            = COLOR_BLUE;
	uiData->bg            = COLOR_WHITE;
	uiData->hasBackground = true;
	uiData->outlineColor  = COLOR_BLACK;
	uiData->outlined      = true;

	//Info Box Members
	for (s32 i = TEAM_SELECT_UI_INFO_BOX_MEMBER_START; i < TEAM_SELECT_UI_INFO_BOX_MEMBER_END; i++) {
		uiData = &data->uiData[i];

		uiData->type = UI_TYPE_TEXT;
		uiData->fg = COLOR_BLACK;
	}
	
	//Back Button
	uiData = &data->uiData[TEAM_SELECT_UI_BACK];
	UI_SetupBackButton(uiData);
	
	//View Roster 
	uiData = &data->uiData[TEAM_SELECT_UI_PREVIEW];
	UI_SetupDefaultButton(uiData);
	uiData->hidden = true;
	
	//Continue
	uiData = &data->uiData[TEAM_SELECT_UI_CONTINUE];
	UI_SetupButton(uiData, COLOR_BLACK, COLOR_GREEN);
	uiData->hidden = true;
}

static void TeamSelect_AssignOnClickFuncs(TeamSelectData *data)
{
	data->uiData[TEAM_SELECT_UI_RANDOM].onClick = TeamSelect_RandomButton_OnClick;
	data->uiData[TEAM_SELECT_UI_BLACK].onClick = TeamSelect_BlackButton_OnClick;
	data->uiData[TEAM_SELECT_UI_WHITE].onClick = TeamSelect_WhiteButton_OnClick;
	data->uiData[TEAM_SELECT_UI_GREEN].onClick = TeamSelect_GreenButton_OnClick;
	data->uiData[TEAM_SELECT_UI_RED].onClick = TeamSelect_RedButton_OnClick;
	data->uiData[TEAM_SELECT_UI_PINK].onClick = TeamSelect_PinkButton_OnClick;
	data->uiData[TEAM_SELECT_UI_BROWN].onClick = TeamSelect_BrownButton_OnClick;
	data->uiData[TEAM_SELECT_UI_YELLOW].onClick = TeamSelect_YellowButton_OnClick;
	data->uiData[TEAM_SELECT_UI_ORANGE].onClick = TeamSelect_OrangeButton_OnClick;
	data->uiData[TEAM_SELECT_UI_BLUE].onClick = TeamSelect_BlueButton_OnClick;
	
	data->uiData[TEAM_SELECT_UI_BACK].onClick = TeamSelect_BackButton_OnClick;
	data->uiData[TEAM_SELECT_UI_CONTINUE].onClick = TeamSelect_ContinueButton_OnClick;
}

static void TeamSelect_ResizeLayout(UIData *data, const Vector2 windowSize)
{
	f32 wX = (f32)windowSize.x;
	f32 wY = (f32)windowSize.y;

	SDL_FRect *dest = nullptr;

	//Title
	dest = &data[TEAM_SELECT_UI_TITLE].destRect;

	dest->w = wX * 0.5f;
	dest->h = wY * 0.20f;
	dest->x = (wX - dest->w) * 0.5f;
	dest->y = wY * 0.05f;

	//TEAM BUTTONS
	
	SDL_FRect buttonRowArea = {};
	buttonRowArea.x = wX * 0.1f;
	buttonRowArea.y = wY * 0.25f;
	buttonRowArea.w = wX * 0.8f;
	buttonRowArea.h = wY * 0.1f;

	s32 numButtonsInRow = TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_END - TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_START;
	s32 numSpaces = numButtonsInRow - 1;

	f32 spacesW = (buttonRowArea.w * 0.2f) / (f32)numSpaces;
	
	//Team Button Row 1
	for (s32 i = TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_START; i < TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_END; i++) {

		dest = &data[i].destRect;

		dest->h = buttonRowArea.h;
		dest->y = buttonRowArea.y;
		dest->w = (buttonRowArea.w * 0.8f) / (f32)numButtonsInRow;
		if (i == TEAM_SELECT_UI_TEAM_BUTTON_ROW_1_START) {
			dest->x = buttonRowArea.x;
		} else {
			dest->x = data[i - 1].destRect.x + dest->w + spacesW;
		}
	}

	//Adjust Y value for 2 row
	buttonRowArea.y = wY * 0.40f;

	//Team Button Row 2
	for (s32 i = TEAM_SELECT_UI_TEAM_BUTTON_ROW_2_START; i < TEAM_SELECT_UI_TEAM_BUTTON_ROW_2_END; i++) {

		dest = &data[i].destRect;

		dest->h = buttonRowArea.h;
		dest->y = buttonRowArea.y;
		dest->w = (buttonRowArea.w * 0.8f) / (f32)numButtonsInRow;
		if (i == TEAM_SELECT_UI_TEAM_BUTTON_ROW_2_START) {
			dest->x = buttonRowArea.x;
		} else {
			dest->x = data[i - 1].destRect.x + dest->w + spacesW;
		}
	}

	//Info Box
	dest = &data[TEAM_SELECT_UI_INFO_BOX].destRect;

	dest->x = wX * 0.1f;
	dest->y = wY * 0.55f;
	dest->w = wX * 0.8f;
	dest->h = wY * 0.25f;

	dest = nullptr;

	//Info Box Members
	TeamSelect_ResizeInfoBoxMembers(data);

	//Back
	data[TEAM_SELECT_UI_BACK].destRect = UI_GetBackButtonDestRect(wX, wY);
	
	dest = &data[TEAM_SELECT_UI_BACK].destRect;

	//Overwrite Y value to make it match other bottom buttons (this will all be changed im sure)
	dest->y = wY - dest->h - (wY * 0.05f);

	//Preview
	dest = &data[TEAM_SELECT_UI_PREVIEW].destRect;
	
	dest->w = wX * 0.1f;
	dest->h = wY * 0.1f;
	dest->x = (wX * 0.5f) - (dest->w * 0.5f);
	dest->y = wY - dest->h - (wY * 0.05f);

	//Continue
	dest = &data[TEAM_SELECT_UI_CONTINUE].destRect;
	
	dest->w = wX * 0.1f;
	dest->h = wY * 0.1f;
	dest->x = (wX) - (dest->w) - (wX * 0.05f);
	dest->y = wY - dest->h - (wY * 0.05f);

}

static void TeamSelect_ResizeInfoBoxMembers(UIData *data)
{
	SDL_FRect infoBox = data[TEAM_SELECT_UI_INFO_BOX].destRect;
	
	f32 infoX = infoBox.x;
	f32 infoY = infoBox.y;
	f32 infoW = infoBox.w;
	f32 infoH = infoBox.h;

	SDL_FRect *dest = nullptr;

	//Title
	dest = &data[TEAM_SELECT_UI_INFO_BOX_TITLE].destRect;

	*dest = UI_GetTitleDestRect(infoW, infoH);
	dest->x += infoX;
	dest->y += infoY;

	//Desc
	dest = &data[TEAM_SELECT_UI_INFO_BOX_DESC].destRect;

	dest->w = infoW * 0.8f;
	dest->h = infoH * 0.1f;
	dest->x = (infoW - dest->w) * 0.5f;
	dest->y = infoH * 0.4f;
	
	dest->x += infoX;
	dest->y += infoY;

	//Pros
	dest = &data[TEAM_SELECT_UI_INFO_BOX_PROS].destRect;

	dest->w = infoW * 0.8f;
	dest->h = infoH * 0.1f;
	dest->x = (infoW - dest->w) * 0.5f;
	dest->y = infoH * 0.6f;
	
	dest->x += infoX;
	dest->y += infoY;

	//Cons
	dest = &data[TEAM_SELECT_UI_INFO_BOX_CONS].destRect;

	dest->w = infoW * 0.8f;
	dest->h = infoH * 0.1f;
	dest->x = (infoW - dest->w) * 0.5f;
	dest->y = infoH * 0.7f;
	
	dest->x += infoX;
	dest->y += infoY;

	//Off
	dest = &data[TEAM_SELECT_UI_INFO_BOX_OFF].destRect;

	dest->w = infoW * 0.4f;
	dest->h = infoH * 0.1f;
	dest->x = infoW * 0.05f;
	dest->y = infoH * 0.8f;
	
	dest->x += infoX;
	dest->y += infoY;
	
	//Def
	dest = &data[TEAM_SELECT_UI_INFO_BOX_DEF].destRect;

	dest->w = infoW * 0.4f;
	dest->h = infoH * 0.1f;
	dest->x = (infoW * 0.5f) + (infoW * 0.05f);
	dest->y = infoH * 0.8f;
	
	dest->x += infoX;
	dest->y += infoY;
}

static void TeamSelect_CreateTextures(const GameEngine *eng, TeamSelectData *data)
{
	for (s32 i = TEAM_SELECT_UI_START; i < TEAM_SELECT_UI_END; i++) {
		data->uiData[i].texture = Text_CreateUITexture(eng, data->uiStrings[i], &data->uiData[i]);
	}
}

static void TeamSelect_UpdateInfoBoxTextures(const GameEngine *eng, TeamSelectData *data, const TeamID id)
{
	if (id != TEAM_ID_RANDOM) {
		TeamSelect_UpdateInfoBoxMembersTextures(eng, data);
	} else {
		UIData *uiData = &data->uiData[TEAM_SELECT_UI_INFO_BOX];
		uiData->texture = Text_CreateUITexture(eng, data->uiStrings[TEAM_SELECT_UI_INFO_BOX], uiData);
	}

	data->updateInfoBox = false;
}

static void TeamSelect_UpdateInfoBoxMembersTextures(const GameEngine *eng, TeamSelectData *data)
{
	UIData *uiData = nullptr;
	
	for (s32 i = TEAM_SELECT_UI_INFO_BOX_MEMBER_START; i < TEAM_SELECT_UI_INFO_BOX_MEMBER_END; i++) {
		uiData = &data->uiData[i];
		uiData->texture = Text_CreateUITexture(eng, data->uiStrings[i], uiData);
	}
}

static void TeamSelect_UpdateTitleTexture(const GameEngine *eng, TeamSelectData *data)
{
	UIData *uiData = &data->uiData[TEAM_SELECT_UI_TITLE];
	uiData->texture = Text_CreateUITexture(eng, data->uiStrings[TEAM_SELECT_UI_TITLE], uiData);

	data->updateTitle = false;
}

static void TeamSelect_CheckButtonHighlight(UIData *uiData, const FVector2 mousePos)
{
	for (s32 i = TEAM_SELECT_UI_BUTTON_START; i < TEAM_SELECT_UI_BUTTON_END; i++) {
		UIData *data = &uiData[i];
		if (data->hidden) {
			continue;
		}
		UI_UpdateHover(data, mousePos);
	}
}

static TeamSelectUIElement TeamSelect_CheckButtonClick(UIData *uiData, const FVector2 mousePos)
{
	for (s32 i = TEAM_SELECT_UI_BUTTON_START; i < TEAM_SELECT_UI_BUTTON_END; i++) {
		 if (UI_CheckClick(&uiData[i], mousePos)) {
			 return i;
		 }
	}
	return TEAM_SELECT_UI_NONE;
}

//Currently TEAM_ID_NONE is OOB (you can't remove a focus via this func, maybe will change)
static void TeamSelect_UpdateFocusTeam(GameData *data, TeamID id)
{
	TeamID currFocus = data->teamAssignment.focus;
	TeamSelectData *teamSelectData = data->stateData;
	UIData *uiData = nullptr;

	//User clicked the button for the team already in focus
	if (currFocus == id) {
		return;
	}

	//Clear all strings
	teamSelectData->uiStrings[TEAM_SELECT_UI_INFO_BOX] = nullptr;

	for (s32 i = TEAM_SELECT_UI_INFO_BOX_MEMBER_START; i < TEAM_SELECT_UI_INFO_BOX_MEMBER_END; i++) {
		teamSelectData->uiStrings[i] = nullptr;
	}

	//Clear info box texture if it exists
	uiData = &teamSelectData->uiData[TEAM_SELECT_UI_INFO_BOX];
	if (uiData->texture) {
		SDL_DestroyTexture(uiData->texture);
		uiData->texture = nullptr;
	}

	uiData->bg = COLOR_WHITE;
	uiData->hasBackground = true;
	
	//Clear info box member textures if they exist
	for (s32 i = TEAM_SELECT_UI_INFO_BOX_MEMBER_START; i < TEAM_SELECT_UI_INFO_BOX_MEMBER_END; i++) {
		uiData = &teamSelectData->uiData[i];
		if (uiData->texture) {
			SDL_DestroyTexture(uiData->texture);
			uiData->texture = nullptr;
		}
	}

	//Confirm new ID is in range
	if (id <= TEAM_ID_NONE || id >= TEAM_ID_COUNT) {
		//ERROR
		printf("TeamID OOB in TeamSelect_UpdateFocusTeam()\n");
		return;
	}

	//Either load new info box texture (RANDOM) OR new member textures (anything else)
	if (id == TEAM_ID_RANDOM) {
		TeamSelect_LoadRandomInfoBox(teamSelectData);
	} else if (id != TEAM_ID_NONE) {
		TeamSelect_LoadTeamInfoBox(teamSelectData, id);
	} else {
		if (data->teamAssignment.player == TEAM_ID_NONE) {
			TeamSelect_ResetPlayerTeamSelection(teamSelectData);
		} else {
			TeamSelect_SetupCPUTeamSelection(teamSelectData);
		}
	}

	//Set flag to update textures during render phase (hacky I know)
	teamSelectData->updateInfoBox = true;

	//Update nav button visibility
	uiData = &teamSelectData->uiData[TEAM_SELECT_UI_PREVIEW];

	if (id == TEAM_ID_RANDOM) {
		uiData->hidden = true;
	} else {
		uiData->hidden = false;
	}
	
	uiData = &teamSelectData->uiData[TEAM_SELECT_UI_CONTINUE];
	uiData->hidden = false;

	//Update global variable
	data->teamAssignment.focus = id;
}

static void TeamSelect_LoadRandomInfoBox(TeamSelectData *data)
{
	//Setup String
	data->uiStrings[TEAM_SELECT_UI_INFO_BOX] = "LET FATE DECIDE";
	
	//Setup color
	data->uiData[TEAM_SELECT_UI_INFO_BOX].fg = data->uiData[TEAM_SELECT_UI_RANDOM].bg;
}

static void TeamSelect_LoadTeamInfoBox(TeamSelectData *data, TeamID id)
{
	TeamDescription teamDesc = sTeamDescriptions[id];

	//Setup String
	
	//TODO: Format these
	data->uiStrings[TEAM_SELECT_UI_INFO_BOX_TITLE] = teamDesc.title;
	data->uiStrings[TEAM_SELECT_UI_INFO_BOX_DESC]  = teamDesc.desc;
	data->uiStrings[TEAM_SELECT_UI_INFO_BOX_PROS]  = teamDesc.pros;
	data->uiStrings[TEAM_SELECT_UI_INFO_BOX_CONS]  = teamDesc.cons;
	data->uiStrings[TEAM_SELECT_UI_INFO_BOX_OFF]   = teamDesc.off;
	data->uiStrings[TEAM_SELECT_UI_INFO_BOX_DEF]   = teamDesc.def;

	//Update Color
	
	//NOTE kinda ugly using const 2 because if one or the other changes...yikes
	for (s32 i = TEAM_SELECT_UI_INFO_BOX_MEMBER_START; i < TEAM_SELECT_UI_INFO_BOX_MEMBER_END; i++) {
		data->uiData[i].fg = sTeamButtonColors[id - 2];
	}

	//Show bg if needed
	
	if (id == TEAM_ID_WHITE || id == TEAM_ID_YELLOW) {
		data->uiData[TEAM_SELECT_UI_INFO_BOX].bg = COLOR_BLACK;
		data->uiData[TEAM_SELECT_UI_INFO_BOX].hasBackground = true;
	}

}

static void TeamSelect_ResetPlayerTeamSelection(TeamSelectData *data)
{
	//title
	data->uiStrings[TEAM_SELECT_UI_TITLE] = "SELECT PLAYER TEAM";
	//info box
	data->uiStrings[TEAM_SELECT_UI_INFO_BOX] = "SELECT A TEAM";
	//update flag for render loop (hacky I know)
	data->updateTitle = true;
}

static void TeamSelect_SetupCPUTeamSelection(TeamSelectData *data)
{
	data->uiStrings[TEAM_SELECT_UI_TITLE] = "SELECT CPU TEAM";
	data->uiStrings[TEAM_SELECT_UI_INFO_BOX] = "SELECT A TEAM";
	data->updateTitle = true;
}

//   ###   TEAM BUTTONS ON CLICK   ###

static void TeamSelect_RandomButton_OnClick(GameData *data)
{
	TeamSelect_UpdateFocusTeam(data, TEAM_ID_RANDOM);
}

static void TeamSelect_BlackButton_OnClick(GameData *data)
{
	TeamSelect_UpdateFocusTeam(data, TEAM_ID_BLACK);
}

static void TeamSelect_WhiteButton_OnClick(GameData *data)
{
	TeamSelect_UpdateFocusTeam(data, TEAM_ID_WHITE);
}

static void TeamSelect_GreenButton_OnClick(GameData *data)
{
	TeamSelect_UpdateFocusTeam(data, TEAM_ID_GREEN);
}

static void TeamSelect_RedButton_OnClick(GameData *data)
{
	TeamSelect_UpdateFocusTeam(data, TEAM_ID_RED);
}

static void TeamSelect_PinkButton_OnClick(GameData *data)
{
	TeamSelect_UpdateFocusTeam(data, TEAM_ID_PINK);
}

static void TeamSelect_BrownButton_OnClick(GameData *data)
{
	TeamSelect_UpdateFocusTeam(data, TEAM_ID_BROWN);
}

static void TeamSelect_YellowButton_OnClick(GameData *data)
{
	TeamSelect_UpdateFocusTeam(data, TEAM_ID_YELLOW);
}

static void TeamSelect_OrangeButton_OnClick(GameData *data)
{
	TeamSelect_UpdateFocusTeam(data, TEAM_ID_ORANGE);
}

static void TeamSelect_BlueButton_OnClick(GameData *data)
{
	TeamSelect_UpdateFocusTeam(data, TEAM_ID_BLUE);
}

static void TeamSelect_BackButton_OnClick(GameData *data)
{
	//No player team selected -> Back to Main Menu
	//Player team selected    -> Back to player selection

	if (data->teamAssignment.player == TEAM_ID_NONE) {
		//Not prev to prevent return to preview / summary screen
		RequestGameStateTransition(data, GAME_STATE_MAIN_MENU);
	} else { 
		data->teamAssignment.player = TEAM_ID_NONE;
		TeamSelect_UpdateFocusTeam(data, TEAM_ID_NONE);
	}
}

static void TeamSelect_ContinueButton_OnClick(GameData *data)
{
	if (data->teamAssignment.player == TEAM_ID_NONE) {
		data->teamAssignment.player = data->teamAssignment.focus;
		TeamSelect_UpdateFocusTeam(data, TEAM_ID_NONE);
	}

}


static constexpr f32 HUE_CYCLE_TIME =  5000.0f;

static void TeamSelect_UpdateRainbowColor(UIData *randomButton, u64 *hueBaseTime)
{
	u64 hueCurrTime = SDL_GetTicks();
	u64 hueDeltaTime = hueCurrTime - *hueBaseTime;
	f32 progress = (f32)hueDeltaTime / HUE_CYCLE_TIME;
	if (progress >= 1.0f) {
		*hueBaseTime = hueCurrTime;
		progress = 0.0f;
	}
	SDL_Color rainbowColor = Colors_GetRainbowColor(progress);
	randomButton->bg = rainbowColor;
}
