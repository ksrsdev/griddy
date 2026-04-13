
#include "post_update.h"

#include "match.h"


static void None_PostUpdate(GameEngine *eng, GameData *data);

typedef void (*PostUpdateFunc)(GameEngine *eng, GameData *data);

static const PostUpdateFunc PostUpdateTable[] = {
	[MAIN_STATE_NONE]             = None_PostUpdate,
	[MAIN_STATE_ERROR]            = None_PostUpdate,
	[MAIN_STATE_INTRO]            = None_PostUpdate,
	[MAIN_STATE_MAIN_MENU]        = None_PostUpdate,
	[MAIN_STATE_OPTIONS_MENU]     = None_PostUpdate,
	[MAIN_STATE_TEAM_SELECT]      = None_PostUpdate,
	[MAIN_STATE_PRE_GAME_CONFIRM] = None_PostUpdate,
	[MAIN_STATE_MATCH]            = Match_PostUpdate,
};
	

void Main_PostUpdate(GameEngine *eng, GameData *data)
{
	//Possibly return ASAP if state has no PostUpdate
	
	PostUpdateFunc postUpdateFunc = PostUpdateTable[data->state.curr];
	if (postUpdateFunc) {
		postUpdateFunc(eng, data);
	}

}


static void None_PostUpdate(GameEngine *eng, GameData *data)
{

	(void)eng;
	(void)data;
}
