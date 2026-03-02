#include "button.h"
#include "global.h"
#include "player.h"
#include "roster.h"
#include "roster_menu.h"

Button bRosterMenu_Back;

void InitRosterMenuButtons(void)
{
	bRosterMenu_Back = MakeButton("<- BACK", RED);
}

void DrawRosterMenu(void)
{
	//Check to see if there is a loaded roster, if not return to main menu with error
	if (ctx.previewRoster == NULL || ctx.previewRosterCount == 0) {
		ctx.state = MAIN_GAME_STATE_MAIN_MENU;
		ctx.prevState = MAIN_GAME_STATE_STARTUP;
		TraceLog(LOG_ERROR, "ERROR: ctx.previewRoster not loaded correctly for Roster Menu");
		return;
	}
	//Clear
	ClearBackground(RAYWHITE);
	//Title
	//RosterChart
	//Back Button (etc?)
	//Check Input
}
