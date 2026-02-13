#include "team.h"

#include "raylib.h"

#include <string.h>

static const TeamBlueprint sTeamBlueprints[] = {
	[TEAM_NONE] = {
		.id = TEAM_NONE,
		.name = "Team None :^",
		.desc = "Select a team!",
		.pros = "NONE",
		.cons = "NONE",
		.rosterSchema = {{ 0 }}
	},
	[TEAM_RANDOM] = {
		.id = TEAM_RANDOM,
		.name = "Random Team",
		.desc = "Let fate decide!",
		.pros = "Mystery is fun!.",
		.cons = "You might regret your choice.",
		.rosterSchema = {{ 0 }}
	},
	[TEAM_BLACK] = {
		.id = TEAM_BLACK,
		.name = "BLACK",
		.desc = "Less thinking, more hitting.",
		.pros = "RB, WR, DL, LB, DB",
		.cons = "OL, QB, TE, K",
		.rosterSchema = {
			[POSITION_NONE]             = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_TACKLE]           = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_GUARD]            = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_CENTER]           = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_QUARTER_BACK]     = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_FULL_BACK]        = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_HALF_BACK]        = {1, PLAYER_STAT_MOD_BUFF},
			[POSITION_TIGHT_END]        = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_WIDE_RECEIVER]    = {3, PLAYER_STAT_MOD_BUFF},
			[POSITION_DEFENSIVE_TACKLE] = {1, PLAYER_STAT_MOD_STAR},
			[POSITION_DEFENSIVE_END]    = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_LINE_BACKER]      = {4, PLAYER_STAT_MOD_BUFF},
			[POSITION_DEFENSIVE_BACK]   = {4, PLAYER_STAT_MOD_BUFF},
			[POSITION_KICKER]           = {1, PLAYER_STAT_MOD_DEBUFF}
		}
	},
	[TEAM_WHITE] = {
		.id = TEAM_WHITE,
		.name = "WHITE",
		.desc = "Tactics beat brute force anyday.",
		.pros = "OL, QB, TE, K",
		.cons = "RB, WR, DL, LB, DB",
		.rosterSchema = {
			[POSITION_NONE]             = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_TACKLE]           = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_GUARD]            = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_CENTER]           = {1, PLAYER_STAT_MOD_BUFF},
			[POSITION_QUARTER_BACK]     = {1, PLAYER_STAT_MOD_STAR},
			[POSITION_FULL_BACK]        = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_HALF_BACK]        = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_TIGHT_END]        = {1, PLAYER_STAT_MOD_BUFF},
			[POSITION_WIDE_RECEIVER]    = {3, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_DEFENSIVE_TACKLE] = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_DEFENSIVE_END]    = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_LINE_BACKER]      = {3, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_DEFENSIVE_BACK]   = {4, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_KICKER]           = {1, PLAYER_STAT_MOD_BUFF}
		}
	},
	[TEAM_GREEN] = {
		.id = TEAM_GREEN,
		.name = "GREEN",
		.desc = "X",
		.pros = "QB, WR, LB",
		.cons = "OL, DB, K",
		.rosterSchema = {
			[POSITION_NONE]             = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_TACKLE]           = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_GUARD]            = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_CENTER]           = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_QUARTER_BACK]     = {1, PLAYER_STAT_MOD_BUFF},
			[POSITION_FULL_BACK]        = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_HALF_BACK]        = {1, PLAYER_STAT_MOD_STANDARD},
			[POSITION_TIGHT_END]        = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_WIDE_RECEIVER]    = {4, PLAYER_STAT_MOD_BUFF},
			[POSITION_DEFENSIVE_TACKLE] = {1, PLAYER_STAT_MOD_STANDARD},
			[POSITION_DEFENSIVE_END]    = {2, PLAYER_STAT_MOD_STANDARD},
			[POSITION_LINE_BACKER]      = {4, PLAYER_STAT_MOD_STAR},
			[POSITION_DEFENSIVE_BACK]   = {4, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_KICKER]           = {1, PLAYER_STAT_MOD_DEBUFF}
		}
	},
	[TEAM_RED] = {
		.id = TEAM_RED,
		.name = "RED",
		.desc = "First we score, then we score more.",
		.pros = "All offensive positions",
		.cons = "All defensive positions",
		.rosterSchema = {
			[POSITION_NONE]             = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_TACKLE]           = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_GUARD]            = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_CENTER]           = {1, PLAYER_STAT_MOD_BUFF},
			[POSITION_QUARTER_BACK]     = {1, PLAYER_STAT_MOD_BUFF},
			[POSITION_FULL_BACK]        = {1, PLAYER_STAT_MOD_BUFF},
			[POSITION_HALF_BACK]        = {1, PLAYER_STAT_MOD_STAR},
			[POSITION_TIGHT_END]        = {1, PLAYER_STAT_MOD_BUFF},
			[POSITION_WIDE_RECEIVER]    = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_DEFENSIVE_TACKLE] = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_DEFENSIVE_END]    = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_LINE_BACKER]      = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_DEFENSIVE_BACK]   = {5, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_KICKER]           = {1, PLAYER_STAT_MOD_STANDARD}
		}
	},
	[TEAM_PINK] = {
		.id = TEAM_PINK,
		.name = "PINK",
		.desc = "Who needs to hit hard when you can outrun the competion?",
		.pros = "WR, DB",
		.cons = "OL, DL",
		.rosterSchema = {
			[POSITION_NONE]             = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_TACKLE]           = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_GUARD]            = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_CENTER]           = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_QUARTER_BACK]     = {1, PLAYER_STAT_MOD_STANDARD},
			[POSITION_FULL_BACK]        = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_HALF_BACK]        = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_TIGHT_END]        = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_WIDE_RECEIVER]    = {5, PLAYER_STAT_MOD_STAR},
			[POSITION_DEFENSIVE_TACKLE] = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_DEFENSIVE_END]    = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_LINE_BACKER]      = {2, PLAYER_STAT_MOD_STANDARD},
			[POSITION_DEFENSIVE_BACK]   = {6, PLAYER_STAT_MOD_BUFF},
			[POSITION_KICKER]           = {1, PLAYER_STAT_MOD_STANDARD}
		}
	},
	[TEAM_BROWN] = {
		.id = TEAM_BROWN,
		.name = "BROWN",
		.desc = "This ain't your granpa's football team, it's your great-grandpa's.",
		.pros = "OL, DL",
		.cons = "QB, DB",
		.rosterSchema = {
			[POSITION_NONE]             = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_TACKLE]           = {2, PLAYER_STAT_MOD_STAR},
			[POSITION_GUARD]            = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_CENTER]           = {1, PLAYER_STAT_MOD_BUFF},
			[POSITION_QUARTER_BACK]     = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_FULL_BACK]        = {1, PLAYER_STAT_MOD_STANDARD},
			[POSITION_HALF_BACK]        = {2, PLAYER_STAT_MOD_STANDARD},
			[POSITION_TIGHT_END]        = {2, PLAYER_STAT_MOD_STANDARD},
			[POSITION_WIDE_RECEIVER]    = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_DEFENSIVE_TACKLE] = {4, PLAYER_STAT_MOD_BUFF},
			[POSITION_DEFENSIVE_END]    = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_LINE_BACKER]      = {2, PLAYER_STAT_MOD_STANDARD},
			[POSITION_DEFENSIVE_BACK]   = {3, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_KICKER]           = {1, PLAYER_STAT_MOD_STANDARD}
		}
	},
	[TEAM_YELLOW] = {
		.id = TEAM_YELLOW,
		.name = "YELLOW",
		.desc = "Twin Tight End Big Bodies",
		.pros = "OL, TE, DL",
		.cons = "WR, DB",
		.rosterSchema = {
			[POSITION_NONE]             = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_TACKLE]           = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_GUARD]            = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_CENTER]           = {1, PLAYER_STAT_MOD_BUFF},
			[POSITION_QUARTER_BACK]     = {1, PLAYER_STAT_MOD_STANDARD},
			[POSITION_FULL_BACK]        = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_HALF_BACK]        = {1, PLAYER_STAT_MOD_STANDARD},
			[POSITION_TIGHT_END]        = {2, PLAYER_STAT_MOD_STAR},
			[POSITION_WIDE_RECEIVER]    = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_DEFENSIVE_TACKLE] = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_DEFENSIVE_END]    = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_LINE_BACKER]      = {2, PLAYER_STAT_MOD_STANDARD},
			[POSITION_DEFENSIVE_BACK]   = {5, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_KICKER]           = {1, PLAYER_STAT_MOD_STANDARD}
		}
	},
	[TEAM_ORANGE] = {
		.id = TEAM_ORANGE,
		.name = "ORANGE",
		.desc = "Wing T",
		.pros = "OL, FB, DB",
		.cons = "QB, WR, K",
		.rosterSchema = {
			[POSITION_NONE]             = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_TACKLE]           = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_GUARD]            = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_CENTER]           = {1, PLAYER_STAT_MOD_BUFF},
			[POSITION_QUARTER_BACK]     = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_FULL_BACK]        = {1, PLAYER_STAT_MOD_STAR},
			[POSITION_HALF_BACK]        = {2, PLAYER_STAT_MOD_STANDARD},
			[POSITION_TIGHT_END]        = {1, PLAYER_STAT_MOD_STANDARD},
			[POSITION_WIDE_RECEIVER]    = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_DEFENSIVE_TACKLE] = {3, PLAYER_STAT_MOD_STANDARD},
			[POSITION_DEFENSIVE_END]    = {2, PLAYER_STAT_MOD_STANDARD},
			[POSITION_LINE_BACKER]      = {2, PLAYER_STAT_MOD_STANDARD},
			[POSITION_DEFENSIVE_BACK]   = {4, PLAYER_STAT_MOD_BUFF},
			[POSITION_KICKER]           = {1, PLAYER_STAT_MOD_DEBUFF}
		}
	},
	[TEAM_BLUE] = {
		.id = TEAM_BLUE,
		.name = "BLUE",
		.desc = "You can't lose if your opponent can't score.",
		.pros = "All defensive positions.",
		.cons = "All offensive positions.",
		.rosterSchema = {
			[POSITION_NONE]             = {0, PLAYER_STAT_MOD_STANDARD},
			[POSITION_TACKLE]           = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_GUARD]            = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_CENTER]           = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_QUARTER_BACK]     = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_FULL_BACK]        = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_HALF_BACK]        = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_TIGHT_END]        = {1, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_WIDE_RECEIVER]    = {2, PLAYER_STAT_MOD_DEBUFF},
			[POSITION_DEFENSIVE_TACKLE] = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_DEFENSIVE_END]    = {2, PLAYER_STAT_MOD_BUFF},
			[POSITION_LINE_BACKER]      = {3, PLAYER_STAT_MOD_BUFF},
			[POSITION_DEFENSIVE_BACK]   = {4, PLAYER_STAT_MOD_STAR},
			[POSITION_KICKER]           = {1, PLAYER_STAT_MOD_STANDARD}
		}
	},
};


TeamData gTeamData[TEAM_COUNT];

void InitTeamData(void)  
{
	for (int i=0; i<TEAM_COUNT; i++) {
		gTeamData[i].id   = sTeamBlueprints[i].id;
		gTeamData[i].name = sTeamBlueprints[i].name;
		gTeamData[i].desc = sTeamBlueprints[i].desc;
		gTeamData[i].pros = sTeamBlueprints[i].pros;
		gTeamData[i].cons = sTeamBlueprints[i].cons;
		memcpy(gTeamData[i].rosterSchema, sTeamBlueprints[i].rosterSchema, sizeof(sTeamBlueprints[i].rosterSchema));
		//Color
		switch (i) {
			case TEAM_WHITE:
				gTeamData[i].color = WHITE;
				break;
			case TEAM_GREEN:
				gTeamData[i].color = GREEN;
				break;
			case TEAM_RED:
				gTeamData[i].color = RED;
				break;
			case TEAM_PINK:
				gTeamData[i].color = PINK;
				break;
			case TEAM_BROWN:
				gTeamData[i].color = BROWN;
				break;
			case TEAM_YELLOW:
				gTeamData[i].color = YELLOW;
				break;
			case TEAM_ORANGE:
				gTeamData[i].color = ORANGE;
				break;
			case TEAM_BLUE:
				gTeamData[i].color = BLUE;
				break;
			case TEAM_NONE:
			case TEAM_RANDOM:
			case TEAM_BLACK:
			default:
				gTeamData[i].color = BLACK;
				break;
		}
	}
}

const TeamData* GetTeamData(TeamId id)
{
	return &gTeamData[id];
}
