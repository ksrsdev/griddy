#ifndef GUARD_SCOREBOARD_H
#define GUARD_SCOREBOARD_H

#include "ui.h"

typedef enum {
	SCOREBOARD_UI_NONE,

	SCOREBOARD_UI_PLAYER,       //Team Name
	SCOREBOARD_UI_PLAYER_SCORE, //0-99

	SCOREBOARD_UI_DASH,         //-

	SCOREBOARD_UI_CPU_SCORE,    //0-99
	SCOREBOARD_UI_CPU,          //Team Name

	SCOREBOARD_UI_DOWN,         //1st, 2nd, 3rd, 4th
	SCOREBOARD_UI_AND,          //&
	SCOREBOARD_UI_DISTANCE,     //1-99

	SCOREBOARD_UI_ONTHE,        //On the
	SCOREBOARD_UI_LOS,          //Team (color?)1-49

	SCOREBOARD_UI_PLAY_COUNT,   //0-32
	SCOREBOARD_UI_PLAYS_REMAIN, //Plays Remain

	SCOREBOARD_UI_COUNT
} ScoreboardUIElement;

typedef struct {
	UIData uiData[SCOREBOARD_UI_COUNT];
	const char *uiStrings[SCOREBOARD_UI_COUNT];
} ScoreboardData;

void Scoreboard_UpdateScoreboardLayout(ScoreboardData *scoreboard);

#endif
