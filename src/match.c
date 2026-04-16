#include "match.h"

#include <stdlib.h>

#include "coin_toss.h"
#include "error.h"
#include "init.h"
#include "match_summary.h"
#include "play_calling.h"

static void Match_StateManager(GameEngine *eng, GameData *data);

static void Match_Init_MatchCtx(MatchCtx *matchCtx);
static void Match_Cleanup_MatchStateData(GameEngine *eng, GameData *data);

static void Match_StateNone_Func(GameEngine *eng, GameData *data);
static void Match_StateNone_FuncNoEng(GameData *data);
static void Match_StateNone_FuncConsts(const GameEngine *eng, const GameData *data);

typedef void (*MatchInitFunc)(GameEngine *eng, GameData *data);
typedef void (*MatchCleanupFunc)(GameEngine *eng, GameData *data);
typedef void (*MatchUpdateFunc)(GameData *data);
typedef void (*MatchRenderFunc)(const GameEngine *eng, const GameData *data);

static const MatchInitFunc MatchInitTable[MATCH_STATE_COUNT] = {
	[MATCH_STATE_NONE]         = Match_StateNone_Func,
	[MATCH_STATE_COIN_TOSS]    = CoinToss_Init,
	[MATCH_STATE_PLAY_CALLING] = PlayCalling_Init,
	[MATCH_STATE_SUMMARY]      = Match_Summary_Init,
};

static const MatchCleanupFunc MatchCleanupTable[MATCH_STATE_COUNT] = {
	[MATCH_STATE_NONE]         = Match_StateNone_Func,
	[MATCH_STATE_COIN_TOSS]    = CoinToss_Cleanup,
	[MATCH_STATE_PLAY_CALLING] = PlayCalling_Cleanup,
	[MATCH_STATE_SUMMARY]      = Match_Summary_Cleanup,
};

static const MatchUpdateFunc MatchUpdateTable[MATCH_STATE_COUNT] = {
	[MATCH_STATE_NONE]         = Match_StateNone_FuncNoEng,
	[MATCH_STATE_COIN_TOSS]    = CoinToss_Update,
	[MATCH_STATE_PLAY_CALLING] = PlayCalling_Update,
	[MATCH_STATE_SUMMARY]      = Match_Summary_Update,
};

static const MatchRenderFunc MatchRenderTable[MATCH_STATE_COUNT] = {
	[MATCH_STATE_NONE]         = Match_StateNone_FuncConsts,
	[MATCH_STATE_COIN_TOSS]    = CoinToss_Render,
	[MATCH_STATE_PLAY_CALLING] = PlayCalling_Render,
	[MATCH_STATE_SUMMARY]      = Match_Summary_Render,
};

void Match_Init(GameEngine *eng, GameData *data)
{
	(void)eng;

	//First create match ctx
	data->stateData = calloc(1, sizeof(MatchCtx));
	if (data->stateData == nullptr) {
		//error.c errors are fatal
		Error_Alert(data, ERROR_ALLOC, "matchCtx failed calloc()");
		return;
	}

	MatchCtx *matchCtx = data->stateData;

	//Set initial state for match - note coin toss is init by state manager call
	Match_Init_MatchCtx(matchCtx);
}

void Match_Cleanup(GameEngine *eng, GameData *data)
{

	//Cleanup MatchCtx.matchStateData void pointer - generalized as it's used in Match_StateManager as well
	Match_Cleanup_MatchStateData(eng, data);

	Deinit_StateData(&data->stateData);
}

//UPDATE
void Match_Update(GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	//Switch on game state for update func - copy update.c
	MatchUpdateFunc updateFunc = MatchUpdateTable[matchCtx->state.curr];
	if (updateFunc) {
		updateFunc(data);
	}
}

//POST UPDATE
void Match_PostUpdate(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;

	//State manager
	if (matchCtx->state.curr != matchCtx->state.next) {
		Match_StateManager(eng, data);
	}

	//Update sub states (like if a texture needs updating in whatever substate - put a table here)


}

//RENDER
void Match_Render(const GameEngine *eng, const GameData *data)
{
	MatchCtx *matchCtx = data->stateData;


	//Switch on game state for render func - copy render.c
	MatchRenderFunc renderFunc = MatchRenderTable[matchCtx->state.curr];
	if (renderFunc) {
		renderFunc(eng, data);
	}

}

static void Match_StateManager(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;
	
	//Cleanup current MatchStateData
	Match_Cleanup_MatchStateData(eng, data);
	
	//Assign new state
	matchCtx->state.curr = matchCtx->state.next;
	
	//Init new state
	MatchInitFunc initFunc = MatchInitTable[matchCtx->state.curr];
	if (initFunc) {
		initFunc(eng, data);
	}

}

static void Match_Init_MatchCtx(MatchCtx *matchCtx)
{
	matchCtx->state.curr = MATCH_STATE_NONE;
	matchCtx->state.next = MATCH_STATE_COIN_TOSS;
	matchCtx->matchStateData = nullptr;
}

//Destroy the textures, free the memory, and point to nullptr
static void Match_Cleanup_MatchStateData(GameEngine *eng, GameData *data)
{
	MatchCtx *matchCtx = data->stateData;

	//Since each state UI stuff is different we need idividual functions to cleanup the textures and get the struct ready to be freed - Mirrors the cleanup paradigm for the main state manager
	MatchCleanupFunc cleanupFunc = MatchCleanupTable[matchCtx->state.curr];
	if (cleanupFunc) {
		cleanupFunc(eng, data);
	}

	//After the textures (etc) have been destroy the struct is ready to be freed and pointer ready tobbe set to null. Mirrors Deinit_StateData
	if (matchCtx->matchStateData) {
		free(matchCtx->matchStateData);
		matchCtx->matchStateData = nullptr;
	}

}

static void Match_StateNone_Func(GameEngine *eng, GameData *data)
{
	(void)eng;
	(void)data;
}

static void Match_StateNone_FuncNoEng(GameData *data)
{
	(void)data;
}

static void Match_StateNone_FuncConsts(const GameEngine *eng, const GameData *data)
{
	(void)eng;
	(void)data;
}
