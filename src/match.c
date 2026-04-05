#include "match.h"


static vod Match_InitUIStrings(MatchData *matchData);
static vod Match_InitUIData(const GameEngine *eng, const GameData *data);
static void Match_InitMatchData(MatchData *data);


void Match_Init(GameEngine *eng, GameData *data)
{
	data->stateData = calloc(1, sizeof(MatchData));
	if (data->stateData == nullptr) {
		//error.c errors are fatal
		Error_Alert(data, ERROR_ALLOC, "matchData failed calloc()");
		return;
	}

	MatchData *matchData = data->stateData;

	Match_InitUIStrings(matchData);

	Match_InitUIData(eng, data);

	//Set initial state for match
	Match_InitMatchData(matchData);


}

void Match_Cleanup(GameEngine *eng, GameData *data)
{

	MatchData *matchData = data->stateData;

	//Sub funcs to destroy match state textures
	Match_CoinToss_CleanupTextures(matchData);
	Match_Gameplay_CleanupTextures(matchData);
	Match_Summary_CleanupTextures(matchData);

	Deinit_StateData(&data->stateData);
}

void Match_Update(GameData *data)
{
	//Switch on game state for update func - copy update.c
	

}

void Match_Render(const GameEngine *eng, const GameData *data)
{
	//Switch on game state for render func - copy render.c

}

static vod Match_InitUIStrings(MatchData *matchData)
{
	Match_CoinToss_InitUIStrings(matchData);

	Match_Gameplay_InitUIStrings(matchData);

	Match_Summary_InitUIStrings(matchData);

}

static vod Match_InitUIData(const GameEngine *eng, const GameData *data)
{
	//Assign ui typess and colors
	//Assign on click funcs
	//Resize layout
	//check button highlights
	//create textures
}


static void Match_InitMatchData(MatchData *data)
{
	data->state = MATCH_STATE_COIN_TOSS;
	data->info.playsRemaining = TOTAL_NUM_PLAYS;
}
