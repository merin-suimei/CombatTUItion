#pragma once

#include "skills/Skill.h"

class NoSkill : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override {} //Do nothing
};

class SneakAttack : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    SneakAttack();
};

class StoneSkin : public Skill
{
public:
    void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const override;
public:
    StoneSkin();
};
