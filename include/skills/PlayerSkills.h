#pragma once

#include "skills/Skill.h"

class Poison : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    Poison();
};

class Shield : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    Shield();
};

class CallToArms : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    CallToArms();
};

class Rage : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    Rage();
};
