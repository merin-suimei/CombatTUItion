#pragma once

#include "entities/Contender.h"

enum SkillType
{
    SkillNone, Offensive, Defensive
};

class Skill
{
protected:
    Skill(std::string name, std::string description,
        SkillType type = SkillNone) :
        name(name), description(description), type(type) {}

public:
    virtual void applySkill(
        Attack *attack, const Contender *self,
        const Contender *opponent, int turn) const = 0;
public:
    std::string name;
    std::string description;
    SkillType type = SkillNone;
};
