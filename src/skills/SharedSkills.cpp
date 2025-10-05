#include "skills/SharedSkills.h"

// Sneak Attack
SneakAttack::SneakAttack()
{
    name = "Sneak Attack";
    description = "+1 damage if your Agility higher than opponents'";
    type = Offensive;
}

void SneakAttack::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    if (self->agi > opponent->agi)
        attack->dmg += 1;
}

// Stone Skin
StoneSkin::StoneSkin()
{
    name = "Stone Skin";
    description = "Ignore 1 damage for every Endurance point";
    type = Defensive;
}

void StoneSkin::applySkill(
    Attack *attack, const Contender *self,
    const Contender *opponent, int turn) const
{
    attack->dmg -= self->end;
}
