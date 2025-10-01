#pragma once

#include "entities/Monster.h"
#include "entities/Player.h"
#include "EncounterLog.h"

namespace Combat
{
    EncounterLog simulateEncounter(
        const Player *player, const Monster *monster);
    Attack calculateDamage(const Contender *source, const Contender *target);
}
