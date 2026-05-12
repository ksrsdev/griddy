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

//Duplicates (curr & prev) to remove need to pass PlayResult to Scoreboard_Update
typedef struct {
	//Actual data:
	MatchSession session;
	MatchSession prevSession;

	s32 down;
	s32 distance;
	s32 los;  //0 is player end zone 100 is cpu endzone
	
	s32 prevDown;
	s32 prevDistance;
	s32 prevLos;  //0 is player end zone 100 is cpu endzone

	u8 playsRemaining;
} ScoreboardData;

typedef struct {
	UIData uiData[SCOREBOARD_UI_COUNT];
	const char *uiStrings[SCOREBOARD_UI_COUNT];
	char stringBuffers[SCOREBOARD_UI_COUNT][8];

	bool updateTextures;

	ScoreboardData sbData;
} ScoreboardCtx;

void Scoreboard_Init(GameEngine *eng, ScoreboardCtx *scoreboard, const TeamAssignment teams, const MatchPossession pos);
void Scoreboard_Update(ScoreboardCtx *scoreboard);
void Scoreboard_PostUpdate(GameEngine *eng, ScoreboardCtx *sb);
void Scoreboard_Cleanup(GameEngine *eng, ScoreboardCtx *scoreboard);
void Scoreboard_Render(const GameEngine *eng, UIData *data);

void Scoreboard_ResizeLayout(const SDL_FRect src, ScoreboardCtx *scoreboard, const MatchPossession pos);

#endif
