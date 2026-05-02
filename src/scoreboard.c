#include "scoreboard.h"

#include "colors.h"
#include "text.h"

static void Scoreboard_Init_UIStrings(const char *strings[SCOREBOARD_UI_COUNT], const TeamAssignment teams);
static void Scoreboard_Init_UIData(UIData *data, const TeamAssignment teams, const MatchPossession pos);
static void Scoreboard_Init_UITextures(GameEngine *eng, ScoreboardData *data);

void Scoreboard_Init(GameEngine *eng, ScoreboardData *scoreboard, const TeamAssignment teams, const MatchPossession pos)
{
	Scoreboard_Init_UIStrings(scoreboard->uiStrings, teams);

	Scoreboard_Init_UIData(scoreboard->uiData, teams, pos);

	Scoreboard_Init_UITextures(eng, scoreboard);
}

void Scoreboard_Cleanup(GameEngine *eng, ScoreboardData *scoreboard)
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

static void Scoreboard_Init_UIStrings(const char *strings[SCOREBOARD_UI_COUNT], const TeamAssignment teams)
{
	strings[SCOREBOARD_UI_POSSESSION] = "*";

	//teams
	TeamDescription playerDesc = gTeamDescriptions[teams.player];
	TeamDescription cpuDesc    = gTeamDescriptions[teams.cpu];

	strings[SCOREBOARD_UI_PLAYER_TEAM] = playerDesc.title;
	strings[SCOREBOARD_UI_CPU_TEAM]    = cpuDesc.title;

	strings[SCOREBOARD_UI_PLAYER_SCORE] = "0";
	strings[SCOREBOARD_UI_DASH] = "-";
	strings[SCOREBOARD_UI_CPU_SCORE] = "0";


	strings[SCOREBOARD_UI_DOWN] = "1ST";
	strings[SCOREBOARD_UI_AND] = "&";
	strings[SCOREBOARD_UI_DISTANCE] = "10";

	strings[SCOREBOARD_UI_ONTHE] = "ON THE";
	strings[SCOREBOARD_UI_LOS] = "20";


	strings[SCOREBOARD_UI_PLAY_COUNT] = "32";
	strings[SCOREBOARD_UI_PLAYS_REMAIN] = "PLAYS REMAIN";
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
		ui->fg = playerDesc.color;
	} else if (pos == POSSESSION_CPU) {
		ui->fg = cpuDesc.color;
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

static void Scoreboard_Init_UITextures(GameEngine *eng, ScoreboardData *data)
{
	for (s32 i = SCOREBOARD_UI_START; i < SCOREBOARD_UI_END; i++) {
		UIData *ui = &data->uiData[i];
		ui->texture = Text_CreateUITexture(eng, data->uiStrings[i], ui);
	}
}

void Scoreboard_ResizeLayout(const SDL_FRect dest, ScoreboardData *scoreboard)
{
	(void)dest;

	(void)scoreboard;
}
