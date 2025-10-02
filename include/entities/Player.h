#pragma once

#include "entities/Contender.h"
#include "entities/Weapon.h"
#include <string>

enum PlayerClass
{
    Rouge, Warrior, Barbarian
};

class Player : public Contender
{
public:
    Player(const std::string name, PlayerClass baseClass);

    int getDamage() const override;
    void LevelUp(PlayerClass playerClass);
    std::string ClassToString() const;
public:
    Weapon weapon;
    int lvlRouge = 0;
    int lvlWarrior = 0;
    int lvlBarbarian = 0;
};
