#pragma once

#include "graphics/Redrawable.h"
#include "EncounterLog.h"
#include <curses.h>
#include <memory>

class CombatScreen : public Redrawable
{
private:
    CombatScreen(const EncounterLog log);
    std::vector<std::string> FormatPlayerStats();
    std::vector<std::string> FormatMonsterStats();
private:
    EncounterLog log;

    WINDOW *counter;
    std::string counterText;

    WINDOW *statusPlayer;
    int playerCurrentHP;
    std::vector<std::string> statusTextPlayer;
    WINDOW *damagePlayer;
    WINDOW *skillsPlayer;
    std::vector<std::string> skillsTextPlayer;

    WINDOW *statusMonster;
    int monsterCurrentHP;
    std::vector<std::string> statusTextMonster;
    WINDOW *damageMonster;
    WINDOW *skillsMonster;
    std::vector<std::string> skillsTextMonster;

    WINDOW *controls;
    std::string controlsText;
    int controlsWidth;

public:
    static std::shared_ptr<CombatScreen> Create(const EncounterLog log);
    ~CombatScreen();

    void Open();
    void Redraw() override;
};
