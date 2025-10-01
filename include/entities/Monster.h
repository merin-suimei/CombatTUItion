#pragma once

#include "entities/Contender.h"

enum MonsterType
{
    Goblin, Skeleton, Slime, Ghost, Golem, Dragon
};

class Monster : public Contender
{
public:
    Monster();

    int getDamage() const override;
public:
    const MonsterType type;
    int dmg;
};
