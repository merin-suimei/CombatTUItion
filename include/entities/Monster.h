#pragma once

#include "entities/Contender.h"
#include <map>

enum MonsterType
{
    Goblin, Skeleton, Slime, Ghost, Golem, Dragon
};

class Monster : public Contender
{
public:
    Monster(std::string name, int hp, int dmg, int str, int agi, int end);

    int getDamage() const override;
public:
    int dmg;
};

static const std::map<MonsterType, Monster> monstersTable
{
    {Goblin, Monster("Goblin", 5, 2, 1, 1, 1)},
    {Skeleton, Monster("Skeleton", 10, 2, 2, 2, 1)},
    {Slime, Monster("Slime", 8, 1, 3, 1, 2)},
    {Ghost, Monster("Ghost", 6, 3, 1, 3, 1)},
    {Golem, Monster("Golem", 10, 1, 3, 1, 3)},
    {Dragon, Monster("Dragon", 20, 4, 3, 3, 3)}
};
