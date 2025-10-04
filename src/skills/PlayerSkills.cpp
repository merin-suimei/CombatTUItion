#include "skills/PlayerSkills.h"

// Poison
Poison::Poison()
{
    name = "Poison";
    description = "+1 damage at the end of turn";
    type = Offensive;
}

void Poison::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    attack->dmg += turn - 1;
}

// Call to Arms
CallToArms::CallToArms()
{
    name = "Call to Arms";
    description = "Double weapon's damage during first turn";
    type = Offensive;
}

void CallToArms::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    if (turn == 1)
        attack->dmg += self->getDamage();
}

// Shield
Shield::Shield()
{
    name = "Shield";
    description = "Ignore 3 damage if your str higher than opponents'";
    type = Defensive;
}

void Shield::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    if (self->str > opponent->str)
        attack->dmg -= 3;
}

// Rage
Rage::Rage()
{
    name = "Rage";
    description = "+2 damage during first 3 turns, then -1 damage";
    type = Offensive;
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
