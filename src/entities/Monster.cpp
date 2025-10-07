#include "entities/Monster.h"

//Temp before lookup table
Monster::Monster(const std::string &name, int hp, int dmg,
    int str, int agi, int end, const Weapon &reward, Skill *skill)
{
    this->name = name;
    this->hp = hp;
    this->dmg = dmg;

    this->str = str;
    this->agi = agi;
    this->end = end;

    this->skill = skill;
    this->reward = reward;
}

void Monster::applyAttackSkills(
    Attack *attack, const Contender *opponent, int turn) const
{
    if (skill->type == Offensive)
        skill->applySkill(attack, this, opponent, turn);
}

void Monster::applyDefenceSkills(
    Attack *attack, const Contender *opponent, int turn) const
{
    if (skill->type == Defensive)
        skill->applySkill(attack, this, opponent, turn);
}
