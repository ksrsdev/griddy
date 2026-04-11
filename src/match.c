#include "match.h"

static void Match_StateManager(GameEngine *eng, GameData *data);

static void Match_InitUIStrings(MatchCtx *matchCtx);
static void Match_InitUIData(const GameEngine *eng, const GameData *data);
static void Match_InitMatchData(MatchCtx *matchCtx);


static void MatchState_None_Init(GameEngine *eng, GameData *data);
static void MatchState_None_Cleanup(GameEngine *eng, GameData *data);

typedef void (*MatchInitFunc)(GameEngine *eng, GameData *data);
typedef void (*MatchCleanupFunc)(GameEngine *eng, GameData *data);

static const MatchInitFunc MatchInitTable[MATCH_STATE_COUNT] = {
	[MATCH_STATE_NONE]         = MatchState_None_Init,
	[MATCH_STATE_COIN_TOSS]    = MatchState_CoinToss_Init,
	[MATCH_STATE_PLAY_CALLING] = MatchState_PlayCalling_Init,
	[MATCH_STATE_SUMMARY]      = MatchState_Summary_Init,
	[MATCH_STATE_COUNT]        = MatchState_None_Init,
};

static const MatchCleanupFunc MatchCleanupTable[MATCH_STATE_COUNT] = {
	[MATCH_STATE_NONE]         = MatchState_None_Cleanup,
	[MATCH_STATE_COIN_TOSS]    = MatchState_CoinToss_Cleanup,
	[MATCH_STATE_PLAY_CALLING] = MatchState_PlayCalling_Cleanup,
	[MATCH_STATE_SUMMARY]      = MatchState_Summary_Cleanup,
	[MATCH_STATE_COUNT]        = MatchState_None_Cleanup,
};

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

	MatchCtx *matchCtx = data->stateData;

	//Sub funcs to destroy match state textures - wasteful should be a switch on specific MatchState
	Match_CoinToss_CleanupTextures(matchData);
	Match_Gameplay_CleanupTextures(matchData);
	Match_Summary_CleanupTextures(matchData);

	Deinit_StateData(&data->stateData);
}

void Match_Update(GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	//Switch on game state for update func - copy update.c
	
	
}

void Match_Render(const GameEngine *eng, const GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	
	//Match State Manager - can't go at the bottom of Update because eng is needed to create the textures
	if (matchCtx->state.curr != matchCtx->state.next) {
		Match_StateManager(eng, data);
	}

	//Switch on game state for render func - copy render.c

}

static void Match_StateManager(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	//Cleanup current state
	CleanupFunc cleanupFunc = MatchCleanupTable[matchCtx->state.curr];
	if (cleanupFunc) {
		cleanupFunc(eng, data);
	}

	if (matchCtx->matchStateData) {
		free(matchCtx->matchStateData);
		matchCtx->matchStateData = nullptr;
	}
	
	//Assign new state
	//
	
	//Init new state

}

static void Match_InitMatchCtx(MatchCtx *matchCtx)
{
	matchCtx->state.curr = MATCH_STATE_NONE;
	matchCtx->state.next = MATCH_STATE_NONE;
	matchCtx->matchStateData = nullptr;
}





static void MatchState_None_Init(GameEngine *eng, GameData *data) 
{
	(void)eng;
	(void)data;
}

static void MatchState_None_Cleanup(GameEngine *eng, GameData *data) 
{
	(void)eng;
	(void)data;
}

