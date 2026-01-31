#include <stdbool.h>

#define MAIN_GAME_STATE_STARTUP                       0
#define MAIN_GAME_STATE_MAIN_MENU                     1
#define MAIN_GAME_STATE_TEST_PLAYGROUND               2
#define MAIN_GAME_STATE_QUICK_GAME_PLAYER_TEAM_SELECT 3
#define MAIN_GAME_STATE_QUICK_GAME_CPU_TEAM_SELECT    4
#define MAIN_GAME_STATE_QUICK_GAME_CONFIRM            5

//global variables
extern int mainGameState;

extern bool gameRunning;
