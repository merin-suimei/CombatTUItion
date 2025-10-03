#pragma once

#include "entities/DamageType.h"
#include "Attack.h"
#include <string>

class Contender
{
public:
    virtual int getDamage() const = 0;
    virtual DamageType getDamageType() const = 0;
    virtual void applyAttackSkills(
        Attack *attack, const Contender *opponent, int turn) const = 0;
    virtual void applyDefenceSkills(
        Attack *attack, const Contender *opponent, int turn) const = 0;
public:
    std::string name;
    int hp;
    int str;
    int agi;
    int end;
};
