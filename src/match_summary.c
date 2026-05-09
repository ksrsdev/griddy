#include "match_summary.h"

#include <stdlib.h>
#include <stdio.h>

#include "colors.h"
#include "error.h"
#include "team.h"
#include "text.h"
#include "ui.h"

static void MatchSummary_Init_UI(GameEngine *eng, GameData *data);

static void MatchSummary_Init_UIStrings(MatchSummaryData *data, const MatchSession session);
static void MatchSummary_Init_UIData(UIData *data, const TeamAssignment teams);
static void MatchSummary_Init_UITextures(GameEngine *eng, MatchSummaryData *data);

//INIT
void MatchSummary_Init(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;


	matchCtx->matchStateData = calloc(1, sizeof(MatchSummaryData));
	if (!matchCtx->matchStateData) {
		Error_Alert(data, ERROR_ALLOC, "MatchSummaryData failed calloc()");
		return;
	}

	MatchSummary_Init_UI(eng, data);
}

//CLEANUP
void MatchSummary_Cleanup(GameEngine *eng, GameData *data)
{

	MatchCtx *matchCtx = data->stateData;

	MatchSummaryData *matchSummaryData = matchCtx->matchStateData;

	for (s32 i = MATCH_SUMMARY_UI_START; i < MATCH_SUMMARY_UI_END; i++) {
		UIData *ui = &matchSummaryData->uiData[i];
		if (ui->texture) {
			UI_DestroyTexture(ui);
		}
	}

	(void)eng;
}

//UPDATE
void MatchSummary_Update(GameData *data)
{
	(void)data;
}

//POST UPDATE
void MatchSummary_PostUpdate(GameEngine *eng, MatchCtx *matchCtx)
{
	(void)eng;
	(void)matchCtx;
}

//RENDER
void MatchSummary_Render(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;
}

static void MatchSummary_Init_UI(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	MatchSummaryData *matchSummaryData = matchCtx->matchStateData;

	MatchSummary_Init_UIStrings(matchSummaryData, matchCtx->session);

	MatchSummary_Init_UIData(matchSummaryData->uiData, data->teamAssignment);

	MatchSummary_Init_UITextures(eng, matchSummaryData);
}

static void MatchSummary_Init_UIStrings(MatchSummaryData *data, const MatchSession session)
{

	const char **strings = data->uiStrings;

	//Title
	strings[MATCH_SUMMARY_UI_TITLE] = "GAME OVER";

	//Player Score
	strings[MATCH_SUMMARY_UI_PLAYER_SCORE] = data->playerScore;
	snprintf(data->playerScore, sizeof(data->playerScore), "%d", session.playerScore);

	//Dash
	strings[MATCH_SUMMARY_UI_DASH] = "-";
	
	//CPU Score
	strings[MATCH_SUMMARY_UI_CPU_SCORE] = data->cpuScore;
	snprintf(data->cpuScore, sizeof(data->cpuScore), "%d", session.cpuScore);

	//Desc
	if (session.playerScore > session.cpuScore) {
		strings[MATCH_SUMMARY_UI_DESC] = "PLAYER WIN!";
	} else if (session.playerScore < session.cpuScore) {
		strings[MATCH_SUMMARY_UI_DESC] = "CPU WIN!";
	} else {
		strings[MATCH_SUMMARY_UI_DESC] = "IT'S A TIE!";
	}
	
	//Quit
	strings[MATCH_SUMMARY_UI_QUIT] = "QUIT";
	
	//Play Again
	strings[MATCH_SUMMARY_UI_PLAY_AGAIN] = "PLAY AGAIN";

}

static void MatchSummary_Init_UIData(UIData *data, const TeamAssignment teams)
{
	UIData *ui = nullptr;

	//First set them all to defaults and handle special cases individually below
	for (s32 i = MATCH_SUMMARY_UI_START; i < MATCH_SUMMARY_UI_END; i++) {
		ui = &data[i];
		ui->type = UI_TYPE_TEXT;
		ui->fg = COLOR_BLACK;
	}

	//Handle Special colors etc
	
	//Player Score
	TeamDescription playerDesc = gTeamDescriptions[teams.player];
	ui = &data[MATCH_SUMMARY_UI_PLAYER_SCORE];

	ui->fg = playerDesc.color;
	Colors_TryAddBackground(ui);

	//CPU Score
	TeamDescription cpuDesc = gTeamDescriptions[teams.cpu];
	ui = &data[MATCH_SUMMARY_UI_CPU_SCORE];

	ui->fg = cpuDesc.color;
	Colors_TryAddBackground(ui);

	//Quit
	ui = &data[MATCH_SUMMARY_UI_QUIT];
	UI_SetupBackButton(ui);

	//Play Again
	ui = &data[MATCH_SUMMARY_UI_PLAY_AGAIN];
	UI_SetupButton(ui, COLOR_BLACK, COLOR_GREEN);
}

static void MatchSummary_Init_UITextures(GameEngine *eng, MatchSummaryData *data)
{
	for (s32 i = MATCH_SUMMARY_UI_START; i < MATCH_SUMMARY_UI_END; i++) {
		UIData *ui = &data->uiData[i];
		ui->texture = Text_CreateUITexture(eng, data->uiStrings[i], ui);
	}
}
