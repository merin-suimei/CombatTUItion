#include "skills/MonsterSkills.h"

void Brittle::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    if (opponent->getDamageType() == Impact)
        attack->dmg *= 2;
}

void Amorphous::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    if (opponent->getDamageType() == Slash)
        attack->dmg -= opponent->getDamage();
}

void DragonBreath::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    if (turn % 3 == 0)
        attack->dmg += 3;
}
