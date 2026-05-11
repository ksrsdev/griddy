#ifndef GUARD_MATCH_SUMMARY_H
#define GUARD_MATCH_SUMMARY_H

#include "context.h"

#include "match.h"
#include "ui.h"

typedef enum {
	MATCH_SUMMARY_UI_NONE,

	MATCH_SUMMARY_UI_TITLE,

	MATCH_SUMMARY_UI_PLAYER_SCORE,
	MATCH_SUMMARY_UI_DASH,
	MATCH_SUMMARY_UI_CPU_SCORE,

	MATCH_SUMMARY_UI_DESC,

	MATCH_SUMMARY_UI_QUIT,
	MATCH_SUMMARY_UI_PLAY_AGAIN,

	MATCH_SUMMARY_UI_COUNT,

	//Aliases


	MATCH_SUMMARY_UI_BUTTON_START = MATCH_SUMMARY_UI_QUIT,
	MATCH_SUMMARY_UI_BUTTON_END   = MATCH_SUMMARY_UI_PLAY_AGAIN + 1,
	
	MATCH_SUMMARY_UI_START = MATCH_SUMMARY_UI_NONE + 1,
	MATCH_SUMMARY_UI_END   = MATCH_SUMMARY_UI_COUNT
} MatchSummaryUIElement;

typedef struct {
	UIData uiData[MATCH_SUMMARY_UI_COUNT];
	const char *uiStrings[MATCH_SUMMARY_UI_COUNT];

	char playerScore[4];
	char cpuScore[4];
} MatchSummaryData;

void MatchSummary_Init(GameEngine *eng, GameData *data);
void MatchSummary_Cleanup(GameEngine *eng, GameData *data);
void MatchSummary_Update(GameData *data);
void MatchSummary_PostUpdate(GameEngine *eng, MatchCtx *matchCtx);
void MatchSummary_Render(const GameEngine *eng, const GameData *data);

#endif
