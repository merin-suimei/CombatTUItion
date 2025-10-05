#pragma once

#include "entities/Contender.h"
#include "entities/Weapon.h"
#include "skills/MonsterSkills.h"
#include "skills/SharedSkills.h"

enum MonsterType
{
    Goblin, Skeleton, Slime, Ghost, Golem, Dragon
};

class Monster : public Contender
{
public:
    Monster(std::string name, int hp, int dmg,
        int str, int agi, int end, Weapon reward, Skill *skill);

    int getDamage() const override { return dmg; }
    DamageType getDamageType() const override { return DamageMonster; }
    void applyAttackSkills(
        Attack *attack, const Contender *opponent, int turn) const override;
    void applyDefenceSkills(
        Attack *attack, const Contender *opponent, int turn) const override;
public:
    int dmg;
    Skill *skill;
    Weapon reward;
};

static const std::map<MonsterType, Monster> monstersTable
{
    {Goblin, Monster("Goblin",
        5, 2, 1, 1, 1, weaponsTable.at(Dagger), new NoSkill())},
    {Skeleton, Monster("Skeleton",
        10, 2, 2, 2, 1, weaponsTable.at(Club), new Brittle())},
    {Slime, Monster("Slime",
        8, 1, 3, 1, 2, weaponsTable.at(Spear), new Amorphous())},
    {Ghost, Monster("Ghost",
        6, 3, 1, 3, 1, weaponsTable.at(Sword), new SneakAttack())},
    {Golem, Monster("Golem",
        10, 1, 3, 1, 3, weaponsTable.at(Axe), new StoneSkin())},
    {Dragon, Monster("Dragon",
        20, 4, 3, 3, 3, weaponsTable.at(LegendarySword), new DragonBreath())}
};
