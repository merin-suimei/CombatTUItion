#include "entities/Player.h"

#include "skills/PlayerSkills.h"
#include "skills/SharedSkills.h"
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

Player::~Player()
{
    for (Skill *skill : skills)
        delete skill;
}

int Player::getDamage() const
{
    return weapon.damage;
}

DamageType Player::getDamageType() const
{
    return weapon.damageType;
}

void Player::applyAttackSkills(
    Attack *attack, const Contender *opponent, int turn) const
{
    for (Skill *skill : skills)
        if (skill->type == Offensive)
            skill->applySkill(attack, this, opponent, turn);
}

void Player::applyDefenceSkills(
    Attack *attack, const Contender *opponent, int turn) const
{
    for (Skill *skill : skills)
        if (skill->type == Defensive)
            skill->applySkill(attack, this, opponent, turn);
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
            skills.push_back(new SneakAttack());
            break;
        case 2:
            agi++;
            break;
        case 3:
            skills.push_back(new Poison());
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
            skills.push_back(new CallToArms());
            break;
        case 2:
            skills.push_back(new Shield());
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
            skills.push_back(new Rage());
            break;
        case 2:
            skills.push_back(new StoneSkin());
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
