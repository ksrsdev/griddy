#ifndef GUARD_MATCH_PLAY_CALLING_H
#define GUARD_MATCH_PLAY_CALLING_H

#include "context.h"

void Match_PlayCalling_Init(GameEngine *eng, GameData *data);
void Match_PlayCalling_Cleanup(GameEngine *eng, GameData *data);
void Match_PlayCalling_Update(GameData *data);
void Match_PlayCalling_Render(const GameEngine *eng, const GameData *data);

#endif
