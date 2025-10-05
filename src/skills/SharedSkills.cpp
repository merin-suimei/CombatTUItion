#include "skills/SharedSkills.h"

void SneakAttack::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    if (self->agi > opponent->agi)
        attack->dmg += 1;
}

void StoneSkin::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    attack->dmg -= self->end;
}
