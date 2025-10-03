#pragma once

#include "entities/Monster.h"
#include "entities/Player.h"
#include "Attack.h"
#include <vector>

enum EncounterOutcome
{
    PlayerWon, PlayerLost, Draw
};

struct EncounterLog
{
    const Player *player;
    const Monster *monster;

    std::vector<Attack> attacks;
    int turn = 1;
    EncounterOutcome outcome;
};
