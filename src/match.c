#include "match.h"


static void Match_InitUIStrings(MatchData *matchCtx);
static void Match_InitUIData(const GameEngine *eng, const GameData *data);
static void Match_InitMatchData(MatchCtx *matchCtx);


void Match_Init(GameEngine *eng, GameData *data)
{
	//First create match ctx
	data->stateData = calloc(1, sizeof(MatchCtx));
	if (data->stateData == nullptr) {
		//error.c errors are fatal
		Error_Alert(data, ERROR_ALLOC, "matchCtx failed calloc()");
		return;
	}

	MatchCtx *matchCtx = data->stateData;

	//Set initial state for match
	Match_InitMatchCtx(matchCtx);

	//Then Init the first state - match coin toss
	Match_CoinToss_Init(matchCtx);

}

void Match_Cleanup(GameEngine *eng, GameData *data)
{

	MatchData *matchData = data->stateData;

	//Sub funcs to destroy match state textures - wasteful should be a switch on specific MatchState
	Match_CoinToss_CleanupTextures(matchData);
	Match_Gameplay_CleanupTextures(matchData);
	Match_Summary_CleanupTextures(matchData);

	Deinit_StateData(&data->stateData);
}

void Match_Update(GameData *data)
{
	//Switch on game state for update func - copy update.c
	
	//Match State Manager
	
}

void Match_Render(const GameEngine *eng, const GameData *data)
{
	//Switch on game state for render func - copy render.c

}

static void Match_InitMatchCtx(MatchCtx *matchCtx)
{
	matchCtx->state.curr = MATCH_STATE_COIN_TOSS;
	matchCtx->state.next = MATCH_STATE_COIN_TOSS;
	matchCtx->matchStateData = nullptr;
}

