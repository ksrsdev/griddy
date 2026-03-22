#ifndef GUARD_INIT_H
#define GUARD_INIT_H

#include "context.h"

Context InitContext(void);
bool Init_SDLSubsystems(void);
bool Init_GameEngine(GameEngine *eng);
void Init_GameData(GameEngine *eng, GameData *data);

void Deinit_GameEngine(GameEngine *eng);
void Deinit_SDLSubsystems(void);

void Deinit_StateData(void **data);


#endif
