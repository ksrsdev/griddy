#include "scoreboard.h"

#include <stdio.h>

#include "colors.h"
#include "text.h"

//INIT HELPER FUNCS
static void Scoreboard_Init_SBData(ScoreboardData *sbData, const MatchPossession pos);
static void Scoreboard_Init_UIStrings(ScoreboardCtx *sb, const TeamAssignment teams);
static void Scoreboard_Init_UIData(UIData *data, const TeamAssignment teams, const MatchPossession pos);
static void Scoreboard_Init_UITextures(GameEngine *eng, ScoreboardCtx *data);

//DESTROY TEXTURE HELPER FUNCS
static void Scoreboard_DestroyStaleTextures(ScoreboardCtx *scoreboard, const PlayResult *result);
static void Scoreboard_DestroyScoreTextures(UIData *data);
static bool Scoreboard_DownChanged(const s32 prevDown, const bool firstDown, const bool turnover);
static bool Scoreboard_DistanceChanged(const s32 prevDistance, const PlayResult *result);

static void Scoreboard_SyncStrings(ScoreboardCtx *sb);

//INIT
void Scoreboard_Init(GameEngine *eng, ScoreboardCtx *scoreboard, const TeamAssignment teams, const MatchPossession pos)
{

	Scoreboard_Init_SBData(&scoreboard->sbData, pos);
	
	Scoreboard_Init_UIStrings(scoreboard, teams);

	Scoreboard_Init_UIData(scoreboard->uiData, teams, pos);

	Scoreboard_Init_UITextures(eng, scoreboard);
}

void Scoreboard_Cleanup(GameEngine *eng, ScoreboardCtx *scoreboard)
{
	for (s32 i = SCOREBOARD_UI_START; i < SCOREBOARD_UI_END; i++) {
		UIData *uiData = &scoreboard->uiData[i];
		if (uiData->texture) {
			SDL_DestroyTexture(uiData->texture);
			uiData->texture = nullptr;
		}
	}

	(void)eng;
}

//UPDATE

void Scoreboard_Update(ScoreboardCtx *scoreboard, const PlayResult *result) {
	
	Scoreboard_DestroyStaleTextures(scoreboard, result);

	//Update Strings / sync strings with data (note data was updated by PlayCalling_ApplyResult)
	Scoreboard_SyncStrings(scoreboard);

	//set flag for Scoreboard_PostUpdate()

}

//POST UPDATE


//RENDER
void Scoreboard_Render(const GameEngine *eng, UIData *data)
{
	for (s32 i = SCOREBOARD_UI_START; i < SCOREBOARD_UI_END; i++) {
		UIData *ui = &data[i];
		UI_RenderUIElement(eng, ui);
	}
}

static void Scoreboard_Init_SBData(ScoreboardData *sbData, const MatchPossession pos)
{
	sbData->session.pos = pos;

	sbData->down = 1;
	sbData->distance = 10;

	if (pos == POSSESSION_PLAYER) {
		sbData->los = 20;
	} else {
		sbData->los = 80;
	}

	//NOTE: This should be a constexpr not hard coded, maybe adjustable in options as well
	sbData->playsRemaining = 32;
}

static void Scoreboard_Init_UIStrings(ScoreboardCtx *sb, const TeamAssignment teams)
{

	//teams
	TeamDescription playerDesc = gTeamDescriptions[teams.player];
	TeamDescription cpuDesc    = gTeamDescriptions[teams.cpu];

	const char **strings = sb->uiStrings;

	//Static Strings - Stay the same - never update - no string buffers
	strings[SCOREBOARD_UI_PLAYER_TEAM] = playerDesc.title;
	strings[SCOREBOARD_UI_CPU_TEAM]    = cpuDesc.title;
	strings[SCOREBOARD_UI_POSSESSION] = "*";
	strings[SCOREBOARD_UI_DASH] = "-";
	strings[SCOREBOARD_UI_AND] = "&";
	strings[SCOREBOARD_UI_ONTHE] = "ON THE";
	strings[SCOREBOARD_UI_PLAYS_REMAIN] = "PLAYS REMAIN";

	//Dynamic Strings (aka string buffers needed)
	
	//Link strings to buffers
	strings[SCOREBOARD_UI_PLAYER_SCORE] = sb->stringBuffers[SCOREBOARD_UI_PLAYER_SCORE];
	strings[SCOREBOARD_UI_CPU_SCORE] = sb->stringBuffers[SCOREBOARD_UI_CPU_SCORE];
	strings[SCOREBOARD_UI_DOWN] = sb->stringBuffers[SCOREBOARD_UI_DOWN];
	strings[SCOREBOARD_UI_DISTANCE] = sb->stringBuffers[SCOREBOARD_UI_DISTANCE];
	strings[SCOREBOARD_UI_LOS] = sb->stringBuffers[SCOREBOARD_UI_LOS];
	strings[SCOREBOARD_UI_PLAY_COUNT] = sb->stringBuffers[SCOREBOARD_UI_PLAY_COUNT];

	//Sync String Buffers to sb data
	Scoreboard_SyncStrings(sb);
}

static void Scoreboard_Init_UIData(UIData *data, const TeamAssignment teams, const MatchPossession pos)
{
	UIData *ui = nullptr;

	//Set all of them to be type text, fg black
	for (s32 i = SCOREBOARD_UI_START; i < SCOREBOARD_UI_END; i++) {
		ui = &data[i];
		ui->type = UI_TYPE_TEXT;
		ui->fg = COLOR_BLACK;
	}

	//Set Colors / backgrounds as needed

	//Player Team
	TeamDescription playerDesc = gTeamDescriptions[teams.player];

	ui = &data[SCOREBOARD_UI_PLAYER_TEAM];
	ui->fg = playerDesc.color;
	
	//set background if needed
	if (Colors_NeedsBackground(ui->fg)) {
			ui->bg = COLOR_BLACK;
			ui->hasBackground = true;
	}

	//CPU Team
	TeamDescription cpuDesc = gTeamDescriptions[teams.cpu];

	ui = &data[SCOREBOARD_UI_CPU_TEAM];
	ui->fg = cpuDesc.color;
	
	//set background if needed
	if (Colors_NeedsBackground(ui->fg)) {
			ui->bg = COLOR_BLACK;
			ui->hasBackground = true;
	}

	//Player Score
	ui = &data[SCOREBOARD_UI_PLAYER_SCORE];
	ui->fg = data[SCOREBOARD_UI_PLAYER_TEAM].fg;
	ui->bg = data[SCOREBOARD_UI_PLAYER_TEAM].bg;
	ui->hasBackground = data[SCOREBOARD_UI_PLAYER_TEAM].hasBackground;

	//CPU Score
	ui = &data[SCOREBOARD_UI_CPU_SCORE];
	ui->fg = data[SCOREBOARD_UI_CPU_TEAM].fg;
	ui->bg = data[SCOREBOARD_UI_CPU_TEAM].bg;
	ui->hasBackground = data[SCOREBOARD_UI_CPU_TEAM].hasBackground;

	//LoS
	ui = &data[SCOREBOARD_UI_LOS];

	if (pos == POSSESSION_PLAYER) {
		ui->fg = cpuDesc.color;
	} else if (pos == POSSESSION_CPU) {
		ui->fg = playerDesc.color;
	} else {
		SDL_Log("pos OOB in Scoreboard_Init_UIData");
		ui->fg = COLOR_BLACK;
	}

	//Set background if needed
	if (Colors_NeedsBackground(ui->fg)) {
			ui->bg = COLOR_BLACK;
			ui->hasBackground = true;
	}
}

void Scoreboard_ResizeLayout(const SDL_FRect src, ScoreboardCtx *scoreboard, const MatchPossession pos)
{
	UIData *ui = scoreboard->uiData;;

	SDL_FRect *dest = nullptr;

	//Player Team
	dest = &ui[SCOREBOARD_UI_PLAYER_TEAM].dest;

	dest->w = src.w * 0.25f;
	dest->h = src.h * 0.2f;
	dest->x = src.x + (src.w * 0.125f);
	dest->y = src.y + (src.h * 0.1f);

	//CPU Team
	dest = &ui[SCOREBOARD_UI_CPU_TEAM].dest;

	dest->w = src.w * 0.25f;
	dest->h = src.h * 0.2f;
	dest->x = src.x + src.w - (src.w * 0.125f) - dest->w;
	dest->y = src.y + (src.h * 0.1f);

	//Possession
	dest = &ui[SCOREBOARD_UI_POSSESSION].dest;

	dest->w = src.w * 0.125f;
	dest->h = src.h * 0.1f;
	dest->y = src.y /*+ (src.h * 0.05f)*/;

	UIData *hasPos = nullptr;
	if (pos == POSSESSION_PLAYER) {
		hasPos = &ui[SCOREBOARD_UI_PLAYER_TEAM];
	} else if  (pos == POSSESSION_CPU) {
		hasPos = &ui[SCOREBOARD_UI_CPU_TEAM];
	} else {
		SDL_Log("pos OOB in Scoreboard_ResizeLayout");
		hasPos = &ui[SCOREBOARD_UI_DASH];
	}

	dest->x = hasPos->dest.x + ( (hasPos->dest.w - dest->w) * 0.5f);

	//Player Score
	dest = &ui[SCOREBOARD_UI_PLAYER_SCORE].dest;

	dest->w = src.w * 0.25f;
	dest->h = src.h * 0.2f;
	dest->x = src.x + (src.w * 0.125f);
	dest->y = src.y + (src.h * 0.4f);

	//Dash
	dest = &ui[SCOREBOARD_UI_DASH].dest;

	dest->w = src.w * 0.25f;
	dest->h = src.h * 0.2f;
	dest->x = src.x + (src.w * 0.5f) - (dest->w * 0.5f);
	dest->y = src.y + (src.h * 0.1f);

	//CPU Score
	dest = &ui[SCOREBOARD_UI_CPU_SCORE].dest;

	dest->w = src.w * 0.25f;
	dest->h = src.h * 0.2f;
	dest->x = src.x + src.w - (src.w * 0.125f) - dest->w;
	dest->y = src.y + (src.h * 0.4f);

	//Down
	dest = &ui[SCOREBOARD_UI_DOWN].dest;

	dest->w = src.w * 0.25f * 0.333f;
	dest->h = src.h * 0.2f;
	dest->x = src.x + (src.w * 0.025f);
	dest->y = src.y + (src.h * 0.7f);

	//&
	dest = &ui[SCOREBOARD_UI_AND].dest;

	dest->w = src.w * 0.25f * 0.333f;
	dest->h = src.h * 0.2f;
	dest->x = src.x + ui[SCOREBOARD_UI_DOWN].dest.w + (src.w * 0.025f);
	dest->y = src.y + (src.h * 0.7f);

	//Distance
	dest = &ui[SCOREBOARD_UI_DISTANCE].dest;

	dest->w = src.w * 0.25f * 0.333f;
	dest->h = src.h * 0.2f;
	dest->x = src.x + (ui[SCOREBOARD_UI_DOWN].dest.w * 2) + (src.w * 0.025f);
	dest->y = src.y + (src.h * 0.7f);

	//On The
	dest = &ui[SCOREBOARD_UI_ONTHE].dest;

	dest->w = src.w * 0.25f * 0.5f;
	dest->h = src.h * 0.2f;
	dest->x = src.x + (src.w * 0.5f) - (dest->w);
	dest->y = src.y + (src.h * 0.7f);

	//LOS
	dest = &ui[SCOREBOARD_UI_LOS].dest;

	dest->w = src.w * 0.25f * 0.5f;
	dest->h = src.h * 0.2f;
	dest->x = src.x + (src.w * 0.5f);
	dest->y = src.y + (src.h * 0.7f);

	//# Plays
	dest = &ui[SCOREBOARD_UI_PLAY_COUNT].dest;

	dest->w = src.w * 0.25f * 0.5f;
	dest->h = src.h * 0.2f;
	dest->x = ui[SCOREBOARD_UI_CPU_SCORE].dest.x + (src.w * 0.1f);
	dest->y = src.y + (src.h * 0.7f);

	//Plays Remain
	dest = &ui[SCOREBOARD_UI_PLAYS_REMAIN].dest;

	dest->w = src.w * 0.25f * 0.5f;
	dest->h = src.h * 0.2f;
	dest->x = ui[SCOREBOARD_UI_CPU_SCORE].dest.x + ui[SCOREBOARD_UI_PLAY_COUNT].dest.w + (src.w * 0.1f);
	dest->y = src.y + (src.h * 0.7f);

}

static void Scoreboard_Init_UITextures(GameEngine *eng, ScoreboardCtx *data)
{
	for (s32 i = SCOREBOARD_UI_START; i < SCOREBOARD_UI_END; i++) {
		UIData *ui = &data->uiData[i];
		ui->texture = Text_CreateUITexture(eng, data->uiStrings[i], ui);
	}
}

static void Scoreboard_DestroyStaleTextures(ScoreboardCtx *scoreboard, const PlayResult *result)
{

	UIData *uiData = scoreboard->uiData;
	ScoreboardData *sbData = &scoreboard->sbData;
	
	//scores if there was a score
	if (result->score) {
		Scoreboard_DestroyScoreTextures(scoreboard->uiData);
	}
	
	//Down - Doesn't actually change if you go from 1st to 1st down...
	if (Scoreboard_DownChanged(sbData->down, result->firstDown, result->turnover)) {
		UI_DestroyTexture(&uiData[SCOREBOARD_UI_DOWN]);
	}

	//Distance - destroy the textures anytime the yards to gain
	if (Scoreboard_DistanceChanged(sbData->distance, result)) {
		UI_DestroyTexture(&uiData[SCOREBOARD_UI_DISTANCE]);
	}

	//numPlays always needs to be updated
	UI_DestroyTexture(&uiData[SCOREBOARD_UI_PLAY_COUNT]);

}

static void Scoreboard_DestroyScoreTextures(UIData *data)
{
	//Player Score
	UI_DestroyTexture(&data[SCOREBOARD_UI_PLAYER_SCORE]);

	//CPU Score
	UI_DestroyTexture(&data[SCOREBOARD_UI_CPU_SCORE]);
}

static bool Scoreboard_DownChanged(const s32 prevDown, const bool firstDown, const bool turnover)
{

	//If the original down wasn't 1st down then the down must have changed
	if (prevDown != 1) {
		return true;
	}

	//Make a firstDown from firstDown
	if (firstDown) {
		return false;
	}

	//Turnover on firstDown
	if (turnover) {
		return false;
	}

	return true;
}

static bool Scoreboard_DistanceChanged(const s32 prevDistance, const PlayResult *result)
{
	//if you go from X and 10 to 1st and 10
	if (prevDistance == 10 && result->firstDown) {
		return false;
	}
	
	//if you don't gain any yards
	if (result->yardsGained == 0) {
		return false;
	}
	
	//if you go from X and 10 to a turnover 
	if (prevDistance == 10 && result->turnover) {
		return false;
	}

	return true;
}

//NOTE: this function should be "dumb" just copy from sbData into strings
//      the actual data needs to be updated in PlayCalling_ApplyResult()
//Basically what this func does is copy data into stringBuffer using snprintf()
static void Scoreboard_SyncStrings(ScoreboardCtx *sb)
{
	ScoreboardData *sbData = &sb->sbData;

	//player score
	snprintf(sb->stringBuffers[SCOREBOARD_UI_PLAYER_SCORE], sizeof(sb->stringBuffers[SCOREBOARD_UI_PLAYER_SCORE]), "%d", sbData->session.playerScore);
	
	//cpu score
	snprintf(sb->stringBuffers[SCOREBOARD_UI_CPU_SCORE], sizeof(sb->stringBuffers[SCOREBOARD_UI_CPU_SCORE]), "%d", sbData->session.cpuScore);
	
	//down
	const char *sDown = nullptr;

	switch (sbData->down) {
		case 1:
			sDown = "1ST";
			break;
		case 2:
			sDown = "2ND";
			break;
		case 3:
			sDown = "3RD";
			break;
		case 4:
			sDown = "4TH";
			break;
		default:
			//ERROR!
			sDown = "OOB!";
			break;
	}

	snprintf(sb->stringBuffers[SCOREBOARD_UI_DOWN], sizeof(sb->stringBuffers[SCOREBOARD_UI_DOWN]), "%s", sDown);
	
	//distance
	snprintf(sb->stringBuffers[SCOREBOARD_UI_DISTANCE], sizeof(sb->stringBuffers[SCOREBOARD_UI_DISTANCE]), "%d", sbData->distance);
	
	//los
	u8 los = 0;

	if (sbData->los <= 50) {
		los = (u8)sbData->los;
	} else {
		los = 100 - (u8)sbData->los;
	}

	snprintf(sb->stringBuffers[SCOREBOARD_UI_LOS], sizeof(sb->stringBuffers[SCOREBOARD_UI_DISTANCE]), "%d", los);

	//plays remain
	snprintf(sb->stringBuffers[SCOREBOARD_UI_PLAY_COUNT], sizeof(sb->stringBuffers[SCOREBOARD_UI_PLAY_COUNT]), "%d", sbData->playsRemaining);
}
