#include "entities/Player.h"

#include "skills/PlayerSkills.h"
#include "skills/SharedSkills.h"
#include <random>

Player::Player(const std::string name, PlayerClass baseClass)
{
    this->name = name;

    // Prepare random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 3);

    // Assign random stats
    str = distrib(gen);
    agi = distrib(gen);
    end = distrib(gen);

    hp = 0;
    weapon = weaponsTable.at(
        baseClass == Rouge   ? Dagger :
        baseClass == Warrior ? Sword  :
                               Club  );
    LevelUp(baseClass);
}

Player::~Player()
{
    for (size_t i = 0; i < skillCount; i++)
        delete skills[i];
}

void Player::applyAttackSkills(
    Attack *attack, const Contender *opponent, int turn) const
{
    for (size_t i = 0; i < skillCount; i++)
        if (skills[i]->type == Offensive)
            skills[i]->applySkill(attack, this, opponent, turn);
}

void Player::applyDefenceSkills(
    Attack *attack, const Contender *opponent, int turn) const
{
    for (size_t i = 0; i < skillCount; i++)
        if (skills[i]->type == Defensive)
            skills[i]->applySkill(attack, this, opponent, turn);
}

int Player::getTotalLevel() const
{
    return lvlRouge + lvlWarrior + lvlBarbarian;
}

void Player::LevelUp(PlayerClass playerClass)
{
    if (getTotalLevel() >= MAX_LEVEL)
        return;

    switch (playerClass)
    {
    case Rouge:
        lvlRouge++;
        hp += 4;
        switch (lvlRouge)
        {
        case 1:
            skills[skillCount++] = new SneakAttack();
            break;
        case 2:
            agi++;
            break;
        case 3:
            skills[skillCount++] = new Poison();
            break;

        default: // Do nothing
            break;
        }
        break;
    case Warrior:
        lvlWarrior++;
        hp += 5;
        switch (lvlWarrior)
        {
        case 1:
            skills[skillCount++] = new CallToArms();
            break;
        case 2:
            skills[skillCount++] = new Shield();
            break;
        case 3:
            str++;
            break;

        default: // Do nothing
            break;
        }
        break;
    case Barbarian:
        lvlBarbarian++;
        hp += 6;
        switch (lvlBarbarian)
        {
        case 1:
            skills[skillCount++] = new Rage();
            break;
        case 2:
            skills[skillCount++] = new StoneSkin();
            break;
        case 3:
            end++;
            break;

        default: // Do nothing
            break;
        }
        break;

    default: // Do nothing
        break;
    }

    hp += end;
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
