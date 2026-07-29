#ifndef GUARD_PLAY_CALLING_H
#define GUARD_PLAY_CALLING_H

#include "context.h"

#include "match.h"
#include "scoreboard.h"
#include "ui.h"

void PlayCalling_Init(GameEngine *eng, GameData *data);
void PlayCalling_Cleanup(GameEngine *eng, GameData *data);
void PlayCalling_Update(GameData *data);
void PlayCalling_PostUpdate(GameEngine *eng, MatchCtx *matchCtx);
void PlayCalling_Render(const GameEngine *eng, const GameData *data);

typedef enum {
	PLAY_NONE,

	PLAY_OFF_RUN,
	PLAY_OFF_SHORT_PASS,
	PLAY_OFF_LONG_PASS,
	PLAY_OFF_KNEEL,
	PLAY_OFF_KICK,
	PLAY_OFF_PUNT,

	PLAY_DEF_BASE,
	PLAY_DEF_MAN,
	PLAY_DEF_COVER,
	PLAY_DEF_PREVENT,
	PLAY_DEF_GOAL_LINE,
	PLAY_DEF_BLITZ,

	PLAY_COUNT,

	//Aliases
	
	PLAY_SPECIAL_TEAMS_START = PLAY_OFF_KNEEL,
	PLAY_SPECIAL_TEAMS_END   = PLAY_OFF_PUNT + 1,
	
	PLAY_OFF_START = PLAY_OFF_RUN,
	PLAY_OFF_END   = PLAY_OFF_PUNT + 1,

	PLAY_DEF_START = PLAY_DEF_BASE,
	PLAY_DEF_END   = PLAY_DEF_BLITZ + 1
} PlayID;

typedef enum {
	PLAY_CALLING_UI_NONE,

	PLAY_CALLING_UI_SCOREBOARD,
	PLAY_CALLING_UI_RESULT_TEXT,

	PLAY_CALLING_UI_BUTTON1,
	PLAY_CALLING_UI_BUTTON2,
	PLAY_CALLING_UI_BUTTON3,

	PLAY_CALLING_UI_BUTTON4,
	PLAY_CALLING_UI_BUTTON5,
	PLAY_CALLING_UI_BUTTON6,

	PLAY_CALLING_UI_QUIT,

	PLAY_CALLING_UI_COUNT,

	//Aliases
	
	PLAY_CALLING_UI_START = PLAY_CALLING_UI_NONE + 1,
	PLAY_CALLING_UI_END   = PLAY_CALLING_UI_COUNT,
	
	PLAY_CALLING_PLAY_BUTTONS_START = PLAY_CALLING_UI_BUTTON1,
	PLAY_CALLING_PLAY_BUTTONS_END   = PLAY_CALLING_UI_BUTTON6 + 1,
	
	PLAY_CALLING_PLAY_BUTTONS_ROW1_START = PLAY_CALLING_UI_BUTTON1,
	PLAY_CALLING_PLAY_BUTTONS_ROW1_END   = PLAY_CALLING_UI_BUTTON3 + 1,
	
	PLAY_CALLING_PLAY_BUTTONS_ROW2_START = PLAY_CALLING_UI_BUTTON4,
	PLAY_CALLING_PLAY_BUTTONS_ROW2_END   = PLAY_CALLING_UI_BUTTON6 + 1,
		
	PLAY_CALLING_BUTTONS_START = PLAY_CALLING_UI_BUTTON1,
	PLAY_CALLING_BUTTONS_END   = PLAY_CALLING_UI_QUIT + 1
} PlayCallingUIElement;

constexpr s32 RESULT_STRING_BUFFER_LEN = 256;

typedef struct {
	ScoreboardCtx scoreboard;
	
	//Non scoreboard UI
	UIData uiData[PLAY_CALLING_UI_COUNT];
	const char *uiStrings[PLAY_CALLING_UI_COUNT];

	bool updateTextures;
	
	//string buffers for the text result if I get there
	char resultStringBuffer[RESULT_STRING_BUFFER_LEN];

	//State Specific Vars
	//Player Choice / Offense Choice
	//CPU Choice    / Defense Choice

} PlayCallingData;

typedef enum {
	SCORE_NONE,
	
	SCORE_TOUCHDOWN,
	SCORE_FIELD_GOAL,
	
	SCORE_TOUCHDOWN_DEFENSE,
	SCORE_SAFETY,

	SCORE_COUNT
} PlayScore;

constexpr s32 gScoreTable[SCORE_COUNT] = {
	[SCORE_NONE]              = 0,
	[SCORE_TOUCHDOWN]         = 7,
	[SCORE_FIELD_GOAL]        = 3,
	[SCORE_TOUCHDOWN_DEFENSE] = 7,
	[SCORE_SAFETY]            = 2,
};

//AI Weights
typedef struct {
	s32 run;
	s32 shortPass;
	s32 longPass; 
} OffenseAIWeights;

typedef struct {
	s32 base;
	s32 man; 
	s32 cover;
	s32 prevent;
	s32 goalLine; 
	s32 blitz;
} DefenseAIWeights;

#endif
