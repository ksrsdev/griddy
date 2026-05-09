#include "match_summary.h"

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

void MatchSummary_Cleanup(GameEngine *eng, GameData *data)
{
	(void)eng;
	(void)data;
}

void MatchSummary_Update(GameData *data)
{
	(void)data;
}

void MatchSummary_PostUpdate(GameEngine *eng, MatchCtx *matchCtx)
{
	(void)eng;
	(void)matchCtx;
}

void MatchSummary_Render(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;
}
