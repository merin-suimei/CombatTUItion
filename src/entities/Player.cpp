#include "entities/Player.h"

#include <random>

using String = std::string;

Player::Player(const std::string name)
{
    this->name = name;
    hp = 5; //Temp before classes

    // Prepare random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 3);

    // Assign random stats
    str = distrib(gen);
    agi = distrib(gen);
    end = distrib(gen);
}

int Player::getDamage() const
{
    //Placeholder
    return 3; // return weapon.dmg;
}

std::string Player::ClassToString() const
{
    // Placeholder
    return "No class";
}
