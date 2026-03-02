#ifndef GUARD_TEAM_SELECT_H
#define GUARD_TEAM_SELECT_H

#include "button.h"
#include "text.h"

#define TEAM_SELECT_BUTTONS_ROW1_COUNT 5
#define TEAM_SELECT_BUTTONS_ROW2_COUNT 5

typedef struct {
	TextBox title;
	TextBox infoBox;
	Button backButton;
	Button continueButton;
	Button teamButtonsRow1[TEAM_SELECT_BUTTONS_ROW1_COUNT];
	Button teamButtonsRow2[TEAM_SELECT_BUTTONS_ROW2_COUNT];
} TeamSelectLayout;

void DrawQuickGameTeamSelect(void);
void InitTeamSelectButtons(void);
void ResolveRandomTeam(void);


#endif
