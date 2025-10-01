#pragma once

#include "entities/Contender.h"
#include "EncounterLog.h"

namespace Combat
{
    EncounterLog simulateEncounter(
        const Contender *contender1, const Contender *contender2);
    Attack calculateDamage(const Contender *source, const Contender *target);
}
