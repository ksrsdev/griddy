#ifndef GUARD_MATCH_PREVIEW_H
#define GUARD_MATCH_PREVIEW_H

#include "button.h"
#include "text.h"

typedef struct {
	TextBox title;
	TextBox playerInfoBox;
	TextBox cpuInfoBox;
	Button backButton;
	Button continueButton;
	//info box buttons??
	//options buttons???
} PreGameSummaryScreenLayout;

void DrawQuickGameConfirm(void);
void InitQuickGameConfirmButtons(void);
void QuickGameConfirm_UnloadRosters(void);

#endif
