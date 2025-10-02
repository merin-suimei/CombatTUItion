#include "entities/Player.h"

#include <random>

Player::Player(const std::string name, PlayerClass baseClass)
{
    this->name = name;

    hp = 0;
    weapon = weaponsTable.at(
        baseClass == Rouge   ? Dagger :
        baseClass == Warrior ? Sword  :
                               Club  );
    LevelUp(baseClass);

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
    return weapon.damage;
}

void Player::LevelUp(PlayerClass playerClass)
{
    switch (playerClass)
    {
    case Rouge:
        lvlRouge += 1;
        hp += 4;
        break;
    case Warrior:
        lvlWarrior += 1;
        hp += 5;
        break;
    case Barbarian:
        lvlBarbarian += 1;
        hp += 6;
        break;

    default: // Do nothing
        break;
    }
}

std::string Player::ClassToString() const
{
    std::string str;
    if (lvlRouge)
    {
        str = "Rouge " + std::string(lvlRouge, 'I');
        if (lvlWarrior || lvlBarbarian)
            str += '/';
        else
            return str;
    }

    if (lvlWarrior)
    {
        str += "Warrior " + std::string(lvlWarrior, 'I');
        if (lvlBarbarian)
            str += '/';
        else
            return str;
    }

    if (lvlBarbarian)
    {
        return str + "Barbarian " + std::string(lvlBarbarian, 'I');
    }

    return "No class";
}
