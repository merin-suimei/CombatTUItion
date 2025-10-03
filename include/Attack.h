#pragma once

enum AttackFlag
{
    Normal, Miss
};

struct Attack
{
    int dmg;
    AttackFlag flag;
};
