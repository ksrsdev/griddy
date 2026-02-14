#include "init.h"
#include "global.h"
#include "main_menu.h"
#include "options_menu.h"
#include "quick_game_confirm.h"
#include "team.h"
#include "team_select.h"
#include "test_playground.h"

void InitAll(void)
{
	InitStartup();
	InitTeamData();
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
	ctx.playerTeam = TEAM_NONE;
	ctx.cpuTeam = TEAM_NONE;
	InitTeamSelectButtons();
}

void InitQuickGameConfirm(void) {
	if (ctx.playerTeam == TEAM_RANDOM || ctx.cpuTeam == TEAM_RANDOM) {
		ResolveRandomTeam();
	}
	InitQuickGameConfirmButtons();
}
void InitOptionsMenu(void) {
	InitOptionsMenuButtons();
}

