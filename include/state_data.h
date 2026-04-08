#ifndef GUARD_LAYOUT_H
#define GUARD_LAYOUT_H

#include <SDL3/SDL.h>

#include "error.h"
#include "main_menu.h"
#include "match.h"
#include "options_menu.h"
#include "pre_game_confirm.h"
#include "team_select.h"
#include "text.h"
#include "ui.h"

//ERROR
typedef struct {
	UIData uiData[ERROR_UI_COUNT];
	const char *uiStrings[ERROR_UI_COUNT];
	bool texturesNeedResizing;
} ErrorData;

//INTRO
typedef enum {
	INTRO_STEP_NONE,
	INTRO_STEP_ANIM,       //Text fades in
	INTRO_STEP_TRANSITION, //Text colors flip
	INTRO_STEP_HOLD,       //Text holds flipped colors
	INTRO_STEP_COUNT
} IntroStep;

typedef enum {
	INTRO_ANIM_NONE,
	INTRO_ANIM_ZOOM,
	INTRO_ANIM_SLIDE_NORTH,
	INTRO_ANIM_SLIDE_SOUTH,
	INTRO_ANIM_SLIDE_EAST,
	INTRO_ANIM_SLIDE_WEST,
	INTRO_ANIM_SWIRL,
	INTRO_ANIM_LOOP,
	INTRO_ANIM_COUNT
} IntroAnim;

typedef struct {
	UIData titleData;
	uint64_t startTime;
	IntroStep introStep;
	IntroAnim introAnim;
} IntroData;

//MAIN MENU
typedef struct {
	UIData uiData[MAIN_MENU_UI_COUNT];
	const char *uiStrings[MAIN_MENU_UI_COUNT];
	u64 pulseBaseTime;
	u64 pulseCurrTime;
} MainMenuData;

//OPTIONS MENU
typedef struct {
	UIData uiData[OPTIONS_MENU_UI_COUNT];
	const char *uiStrings[OPTIONS_MENU_UI_COUNT];
} OptionsMenuData;

//TEAM SELECT
typedef struct {
	UIData uiData[TEAM_SELECT_UI_COUNT];
	const char *uiStrings[TEAM_SELECT_UI_COUNT];
	u64 hueBaseTime;
	bool updateInfoBox;
	bool updateTitle;
} TeamSelectData;

typedef struct {
	UIData uiData[PRE_GAME_CONFIRM_UI_COUNT];
	const char *uiStrings[PRE_GAME_CONFIRM_UI_COUNT];
} PreGameConfirmData;

typedef struct {
	MatchCtx ctx;

	UIData coinTossUIData[MATCH_COIN_TOSS_UI_COUNT];
	const char *coinTossStrings[MATCH_COIN_TOSS_UI_COUNT];
	
	UIData playCallingUIData[MATCH_PLAY_CALLING_UI_COUNT];
	const char *playCallingStrings[MATCH_PLAY_CALLING_UI_COUNT];
	
	UIData summaryUIData[MATCH_SUMMARY_UI_COUNT];
	const char *summaryStrings[MATCH_SUMMARY_UI_COUNT];

} MatchData;

#endif
