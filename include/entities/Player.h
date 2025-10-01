#pragma once

#include "entities/Contender.h"
#include <string>

class Player : public Contender
{
public:
    Player(const std::string name);

    int getDamage() const override;
    std::string ClassToString() const;
public:
    // Weapon weapon;
    // int lvlRouge = 0;
    // int lvlWarrior = 0;
    // int lvlBarbarian = 0;
};
