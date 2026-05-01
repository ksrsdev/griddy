#ifndef GUARD_PLAY_CALLING_H
#define GUARD_PLAY_CALLING_H

#include "context.h"

#include "scoreboard.h"
#include "ui.h"

void PlayCalling_Init(GameEngine *eng, GameData *data);
void PlayCalling_Cleanup(GameEngine *eng, GameData *data);
void PlayCalling_Update(GameData *data);
void PlayCalling_Render(const GameEngine *eng, const GameData *data);

typedef enum {
	OFFENSE_NONE,

	OFFENSE_RUN,
	OFFENSE_SHORT_PASS,
	OFFENSE_LONG_PASS,
	OFFENSE_KNEEL,
	OFFENSE_KICK,
	OFFENSE_PUNT,

	OFFENSE_COUNT
} OffensePlays;

typedef enum {
	DEFENSE_NONE,

	DEFENSE_BASE,
	DEFENSE_MAN,
	DEFENSE_COVER,
	DEFENSE_PREVENT,
	DEFENSE_GOAL_LINE,
	DEFENSE_BLITZ,

	DEFENSE_COUNT
} DefensePlays;

typedef enum {
	PLAY_CALLING_UI_NONE,

	PLAY_CALLING_UI_BUTTON1,
	PLAY_CALLING_UI_BUTTON2,
	PLAY_CALLING_UI_BUTTON3,

	PLAY_CALLING_UI_BUTTON4,
	PLAY_CALLING_UI_BUTTON5,
	PLAY_CALLING_UI_BUTTON6,

	PLAY_CALLING_UI_QUIT,

	PLAY_CALLING_UI_COUNT
} PlayCallingUIElement;


typedef struct {
	ScoreboardData scoreboard;
	//Non scoreboard UI
	UIData uiData[PLAY_CALLING_UI_COUNT];
	const char *uiStrings[PLAY_CALLING_UI_COUNT];
	//State Specific Vars
} PlayCallingData;

#endif
