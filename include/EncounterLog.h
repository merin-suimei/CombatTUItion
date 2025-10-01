#pragma once

#include "entities/Monster.h"
#include "entities/Player.h"
#include <vector>

enum AttackFlag
{
    Normal, Miss, DefenceTooHigh
};

enum EncounterOutcome
{
    PlayerWon, PlayerLost, Draw
};

struct Attack
{
    int dmg;
    AttackFlag flag;
};

struct EncounterLog
{
    const Player *player;
    const Monster *monster;

    std::vector<Attack> attacks;
    int turn = 1;
    EncounterOutcome outcome;
};
