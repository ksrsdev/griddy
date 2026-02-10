// This is your "Spreadsheet"
typedef struct {
    TeamID id;
    const char *name;
    const char *pros;
    const char *cons;
    TeamRosterMod rosterSchema[POSITION_COUNT];
} TeamBlueprint;

static const TeamBlueprint BLUEPRINTS[] = {
    [TEAM_BLUE] = {
        .name = "BLUE BOMBERS",
        .rosterSchema = {
            [POSITION_QUARTER_BACK] = {1, PLAYER_STAT_MOD_STAR},
            [POSITION_TACKLE]       = {2, PLAYER_STAT_MOD_BUFF}
        }
    },
    [TEAM_PINK] = {
        .name = "PINK PANTHERS",
        .rosterSchema = {
            [POSITION_WIDE_RECEIVER]= {5, PLAYER_STAT_MOD_BUFF}
        }
    }
};

void InitTeamData(void) {
    for (int i = 0; i < TEAM_COUNT; i++) {
        // 1. Copy the fixed data (names, rosters) from the blueprint
        // We use the blueprint to fill our global gTeamData
        gTeamData[i].id = BLUEPRINTS[i].id;
        gTeamData[i].name = BLUEPRINTS[i].name;
        memcpy(gTeamData[i].rosterSchema, BLUEPRINTS[i].rosterSchema, sizeof(BLUEPRINTS[i].rosterSchema));

        // 2. Handle the "Annoying" Raylib stuff here once
        if (i == TEAM_BLUE) gTeamData[i].color = BLUE;
        if (i == TEAM_PINK) gTeamData[i].color = PINK;
        // ... and so on
    }
}
