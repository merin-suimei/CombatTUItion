#pragma once

#include "EncounterLog.h"

namespace Combat
{
    EncounterLog simulateEncounter(
        const Player *player, const Monster *monster);
    Attack calculateDamage(
        const Contender *source, const Contender *target, int turn);
}
