#pragma once

#include "graphics/Redrawable.h"
#include "EncounterLog.h"
#include <curses.h>
#include <memory>

class CombatScreen : public Redrawable
{
private:
    CombatScreen(const EncounterLog log);

    void DrawPlayerHalfturn(size_t currentTurn);
    void DrawMonsterHalfturn(size_t currentTurn);

    void RestoreWindow(WINDOW *window, int height, int width,
        int textY, int textX, const char *text);

    std::string FormatPlayerStats();
    std::string FormatMonsterStats();
private:
    EncounterLog log;

    WINDOW *counter;
    std::string counterText;

    WINDOW *statusPlayer;
    int playerCurrentHP;
    std::string statusTextPlayer;
    WINDOW *damagePlayer;
    WINDOW *skillsPlayer;
    std::string skillsTextPlayer;

    WINDOW *statusMonster;
    int monsterCurrentHP;
    std::string statusTextMonster;
    WINDOW *damageMonster;
    WINDOW *skillsMonster;
    std::string skillsTextMonster;

    WINDOW *controls;
    std::string controlsText;
    int controlsWidth;

public:
    static std::shared_ptr<CombatScreen> Create(const EncounterLog log);
    ~CombatScreen();

    void Open();
    void Redraw() override;
};
