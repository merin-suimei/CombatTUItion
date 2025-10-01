#pragma once

#include "entities/Contender.h"
#include "entities/Weapon.h"
#include <map>

enum MonsterType
{
    Goblin, Skeleton, Slime, Ghost, Golem, Dragon
};

class Monster : public Contender
{
public:
    Monster(std::string name, int hp, int dmg,
        int str, int agi, int end, Weapon reward);

    int getDamage() const override;
public:
    int dmg;
    Weapon reward;
};

static const std::map<MonsterType, Monster> monstersTable
{
    {Goblin, Monster("Goblin", 5, 2, 1, 1, 1, weaponsTable.at(Dagger))},
    {Skeleton, Monster("Skeleton", 10, 2, 2, 2, 1, weaponsTable.at(Club))},
    {Slime, Monster("Slime", 8, 1, 3, 1, 2, weaponsTable.at(Spear))},
    {Ghost, Monster("Ghost", 6, 3, 1, 3, 1, weaponsTable.at(Sword))},
    {Golem, Monster("Golem", 10, 1, 3, 1, 3, weaponsTable.at(Axe))},
    {Dragon, Monster("Dragon", 20, 4, 3, 3, 3, weaponsTable.at(LegendarySword))}
};
