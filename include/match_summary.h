#ifndef GUARD_MATCH_SUMMARY_H
#define GUARD_MATCH_SUMMARY_H

#include "context.h"

void Match_Summary_Init(GameEngine *eng, GameData *data);
void Match_Summary_Cleanup(GameEngine *eng, GameData *data);
void Match_Summary_Update(GameData *data);
void Match_Summary_Render(const GameEngine *eng, const GameData *data);

#endif
