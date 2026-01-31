#include "init.h"
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
	playerTeamSelected = 0;
	InitTeamSelectButtons();
}

void InitQuickGameConfirm(void) {
	InitQuickGameConfirmButtons();
}

