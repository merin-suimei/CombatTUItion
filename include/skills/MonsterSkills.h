#pragma once

#include "skills/Skill.h"

class Brittle : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    Brittle() : Skill("Brittle",
        "Double damage from Impact", Defensive) {}
};

class Amorphous : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    Amorphous() : Skill("Amorphous",
        "Ignore Slash damage", Defensive) {}
};

class DragonBreath : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    DragonBreath() : Skill("Dragon Breath",
        "Deals 3 damage every 3 turns", Offensive) {}
};
