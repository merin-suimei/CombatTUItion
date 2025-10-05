#pragma once

#include "skills/Skill.h"

class NoSkill : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override {} //Do nothing
public:
    NoSkill() : Skill("", "", SkillNone) {}
};

class SneakAttack : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    SneakAttack() : Skill("Sneak Attack",
        "+1 damage if your Agility higher than opponents'", Offensive) {}
};

class StoneSkin : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    StoneSkin() : Skill("Stone Skin",
        "Ignore 1 damage for every Endurance point", Defensive) {}
};
