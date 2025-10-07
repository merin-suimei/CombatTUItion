#pragma once

#include "entities/Contender.h"
#include "entities/Weapon.h"
#include "skills/Skill.h"

#define TOTAL_CLASSES 3
#define MAX_LEVEL 3

#define MAX_SKILLS 3

enum PlayerClass
{
    NoClass, Rouge, Warrior, Barbarian
};

class Player : public Contender
{
private:
    void ApplyRougeBonus();
    void ApplyWarriorBonus();
    void ApplyBarbarianBonus();

public:
    Player(const std::string name, PlayerClass baseClass);
    ~Player();

    int getDamage() const override { return weapon.damage; }
    DamageType getDamageType() const override { return weapon.damageType; }
    void applyAttackSkills(
        Attack *attack, const Contender *opponent, int turn) const override;
    void applyDefenceSkills(
        Attack *attack, const Contender *opponent, int turn) const override;

    int getTotalLevel() const;
    void LevelUp(PlayerClass playerClass);
    std::string ClassToString() const;
public:
    Weapon weapon;

    int skillCount = 0;
    Skill* skills[MAX_SKILLS];

    int lvlRouge = 0;
    int lvlWarrior = 0;
    int lvlBarbarian = 0;
};
