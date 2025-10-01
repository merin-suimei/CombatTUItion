#pragma once

#include "entities/Contender.h"
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
    const Contender *contender1;
    const Contender *contender2;

    std::vector<Attack> attacks;
    int turn = 1;
    EncounterOutcome outcome;
};
