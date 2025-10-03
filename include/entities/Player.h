#pragma once

#include "entities/Contender.h"
#include "entities/Weapon.h"
#include "skills/Skill.h"
#include "Attack.h"
#include <string>
#include <vector>

enum PlayerClass
{
    Rouge, Warrior, Barbarian
};

class Player : public Contender
{
public:
    Player(const std::string name, PlayerClass baseClass);
    ~Player();

    int getDamage() const override;
    DamageType getDamageType() const override;
    void applyAttackSkills(
        Attack *attack, const Contender *opponent, int turn) const override;
    void applyDefenceSkills(
        Attack *attack, const Contender *opponent, int turn) const override;

    void LevelUp(PlayerClass playerClass);
    std::string ClassToString() const;
public:
    Weapon weapon;
    std::vector<Skill*> skills;

    int lvlRouge = 0;
    int lvlWarrior = 0;
    int lvlBarbarian = 0;
};
