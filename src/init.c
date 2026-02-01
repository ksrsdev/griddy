#include "init.h"
#include "global.h"
#include "main_menu.h"
#include "quick_game_confirm.h"
#include "team_select.h"
#include "test_playground.h"

void InitAll(void)
{
	InitStartup();
	InitTeamSelect();
	InitQuickGameConfirm();
}

void InitStartup(void)
{
	//Test Buttons Array
	TestPlaygroundInitButtons();
	InitMainMenuButtons();
	InitTeamSelect();
}

void InitTeamSelect(void)
{
	griddy.playerTeam = TEAM_SELECTED_NONE;
	griddy.cpuTeam = TEAM_SELECTED_NONE;
	InitTeamSelectButtons();
}

void InitQuickGameConfirm(void) {
	InitQuickGameConfirmButtons();
}

