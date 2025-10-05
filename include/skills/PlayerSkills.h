#pragma once

#include "skills/Skill.h"

class Poison : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    Poison() : Skill("Poison",
        "+1 damage at the end of turn", Offensive) {}
};

class Shield : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    Shield() : Skill("Call to Arms",
        "Double weapon's damage during first turn", Offensive) {}
};

class CallToArms : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    CallToArms() : Skill("Shield",
        "Ignore 3 damage if your Strength higher than opponents'", Defensive) {}
};

class Rage : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    Rage() : Skill("Rage",
        "+2 damage during first 3 turns, then -1 damage", Offensive) {}
};
