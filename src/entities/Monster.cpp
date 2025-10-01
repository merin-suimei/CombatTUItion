#include "entities/Monster.h"

//Temp before lookup table
Monster::Monster() : type(Goblin), dmg(2)
{
    name = "Goblin";
    hp = 5;
    str = 1;
    agi = 1;
    end = 1;
}

int Monster::getDamage() const
{
    return dmg;
}
