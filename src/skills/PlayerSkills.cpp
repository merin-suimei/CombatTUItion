#include "skills/PlayerSkills.h"

void Poison::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    attack->dmg += turn - 1;
}

void CallToArms::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    if (turn == 1)
        attack->dmg += self->getDamage();
}

void Shield::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    if (self->str > opponent->str)
        attack->dmg -= 3;
}

void Rage::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    if (turn <= 3)
        attack->dmg += 2;
    else
        attack->dmg -= 1;
}
