#pragma once

#include "entities/Player.h"
#include "graphics/Redrawable.h"
#include <curses.h>
#include <memory>

class ClassSelector : public Redrawable
{
private:
    ClassSelector(int lvlRouge, int lvlWarrior, int lvlBarbarian);
    const std::string rougeBonuses[MAX_LEVEL] = {
        "[Skill] Sneak Attack\n"
        " +1 damage if your Agility\nhigher than opponents'",
        "[Stat] +1 agi",
        "[Skill] Poison\n"
        "+1 damage at the end of turn"};
    const std::string warriorBonuses[MAX_LEVEL] = {
        "[Skill] Call to Arms\n"
        " Double weapon's damage\nduring first turn",
        "[Skill] Shield\n"
        " Ignore 3 damage if your\nStrength higher than\nopponents'",
        "[Stat] +1 str"};
    const std::string barbarianBonuses[MAX_LEVEL] = {
        "[Skill] Rage\n"
        " +2 damage during first 3\nturns, then -1 damage",
        "[Skill] Stone Skin\n"
        " Ignore 1 damage for every\nEndurance point",
        "[Stat] +1 end"};
private:
    WINDOW *window;

    WINDOW *rouge;
    std::string rougeText;
    WINDOW *warrior;
    std::string warriorText;
    WINDOW *barbarian;
    std::string barbarianText;

public:
    static std::shared_ptr<ClassSelector> Create(
        int lvlRouge, int lvlWarrior, int lvlBarbarian);
    ~ClassSelector();

    PlayerClass OpenAndGetClass();
    void Redraw() override;
};
