#pragma once

#include <string>

class Contender
{
public:
    virtual int getDamage() const = 0;
public:
    std::string name;
    int hp;
    int str;
    int agi;
    int end;
};
