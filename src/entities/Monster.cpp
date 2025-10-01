#include "entities/Monster.h"

//Temp before lookup table
Monster::Monster(std::string name, int hp, int dmg,
    int str, int agi, int end, Weapon reward)
{
    this->name = name;
    this->hp = hp;
    this->dmg = dmg;

    this->str = str;
    this->agi = agi;
    this->end = end;

    this->reward = reward;
}

int Monster::getDamage() const
{
    return dmg;
}
