#pragma once

#include "entities/Monster.h"
#include "entities/Player.h"
#include "Attack.h"
#include <vector>

enum EncounterOutcome
{
    PlayerWon, PlayerLost, Draw
};

enum ContenderInitiative
{
    PlayerFirst, MonsterFirst
};

struct EncounterLog
{
    const Player *player;
    const Monster *monster;

    std::vector<Attack> attacks;
    int turn = 1;
    ContenderInitiative initiative;
    EncounterOutcome outcome;
};
