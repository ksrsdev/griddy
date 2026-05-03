#ifndef GUARD_SCOREBOARD_H
#define GUARD_SCOREBOARD_H

#include "match.h"
#include "ui.h"

typedef enum {
	SCOREBOARD_UI_NONE,

	SCOREBOARD_UI_POSSESSION,

	SCOREBOARD_UI_PLAYER_TEAM,  //Team Name
	SCOREBOARD_UI_CPU_TEAM,     //Team Name
	
	SCOREBOARD_UI_PLAYER_SCORE, //0-99
	SCOREBOARD_UI_DASH,         //-
	SCOREBOARD_UI_CPU_SCORE,    //0-99

	SCOREBOARD_UI_DOWN,         //1st, 2nd, 3rd, 4th
	SCOREBOARD_UI_AND,          //&
	SCOREBOARD_UI_DISTANCE,     //1-99

	SCOREBOARD_UI_ONTHE,        //On the
	SCOREBOARD_UI_LOS,          //Team (color?)1-49

	SCOREBOARD_UI_PLAY_COUNT,   //0-32
	SCOREBOARD_UI_PLAYS_REMAIN, //Plays Remain

	SCOREBOARD_UI_COUNT,

	//Aliases
	
	SCOREBOARD_UI_START = SCOREBOARD_UI_NONE + 1,
	SCOREBOARD_UI_END   = SCOREBOARD_UI_COUNT
} ScoreboardUIElement;

typedef struct {
	UIData uiData[SCOREBOARD_UI_COUNT];
	const char *uiStrings[SCOREBOARD_UI_COUNT];

	//Actual data:
	s32 playerScore;
	s32 cpuScore;
	s32 down;
	s32 distance;
	s32 los; //1 - 99 convert on the fly
	s32 playsRemaining;
} ScoreboardData;

void Scoreboard_Init(GameEngine *eng, ScoreboardData *scoreboard, const TeamAssignment teams, const MatchPossession pos);
void Scoreboard_Cleanup(GameEngine *eng, ScoreboardData *scoreboard);
void Scoreboard_Render(const GameEngine *eng, UIData *data);

void Scoreboard_ResizeLayout(const SDL_FRect src, ScoreboardData *scoreboard, const MatchPossession pos);

#endif
