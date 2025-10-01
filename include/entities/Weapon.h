#pragma once

#include <map>
#include <string>

enum WeaponType
{
    Sword, Club, Dagger, Axe, Spear, LegendarySword
};

enum DamageType
{
    Slash, Puncture, Impact
};

struct Weapon
{
    std::string name;
    DamageType damageType;
    int damage;
};

static const std::map<WeaponType, Weapon> weaponsTable
{
    {Sword, Weapon{.name = "Sword", .damageType = Slash, .damage = 3}},
    {Club, Weapon{.name = "Club", .damageType = Impact, .damage = 3}},
    {Dagger, Weapon{.name = "Dagger", .damageType = Puncture, .damage = 2}},
    {Axe, Weapon{.name = "Axe", .damageType = Slash, .damage = 4}},
    {Spear, Weapon{.name = "Spear", .damageType = Puncture, .damage = 3}},
    {LegendarySword, Weapon{
        .name = "Legendary Sword", .damageType = Slash, .damage = 10}}
};
