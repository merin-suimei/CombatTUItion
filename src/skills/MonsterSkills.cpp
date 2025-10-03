#include "skills/MonsterSkills.h"

// Brittle
Brittle::Brittle()
{
    name = "Brittle";
    description = "Double damage from Impact";
    type = Defensive;
}

void Brittle::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    if (opponent->getDamageType() == Impact)
        attack->dmg *= 2;
}

// Amorphous
Amorphous::Amorphous()
{
    name = "Amorphous";
    description = "Ignore Slash damage";
    type = Defensive;
}

void Amorphous::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    if (opponent->getDamageType() == Slash)
        attack->dmg -= opponent->getDamage();
}

// Dragon Breath
DragonBreath::DragonBreath()
{
    name = "Dragon Breath";
    description = "Deals 3 damage 3 turns";
    type = Offensive;
}

void DragonBreath::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    if (turn % 3 == 0)
        attack->dmg += 3;
}
