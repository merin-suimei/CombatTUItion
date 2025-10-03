#include "Game.h"

#include "entities/Monster.h"
#include "graphics/CombatScreen.h"
#include "graphics/PopupWindow.h"
#include "graphics/WindowManager.h"
#include "Combat.h"
#include <curses.h>
#include <format>
#include <random>

Game::Game()
{
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    curs_set(0);
}

Game::~Game()
{
    endwin();
}

void Game::Start()
{
    // Prepare random
    std::random_device rd;
    std::mt19937 gen(rd());

    // Hardcoded (First, Last)
    std::uniform_int_distribution<> monsterDist(Goblin, Dragon);

    running = true;
    while (running)
    {
        {
            Monster monster = monstersTable.at(MonsterType(monsterDist(gen)));
            EncounterLog log = Combat::simulateEncounter(&player, &monster);

            std::shared_ptr<CombatScreen> window = CombatScreen::Create(log);
            window->Open();

            std::string popupText =
                log.outcome == PlayerWon  ? player.name + " won"  :
                log.outcome == PlayerLost ? monster.name + " won" :
                log.outcome == Draw       ? "Draw"                :
                                            "Unknown"             ;
            std::shared_ptr<PopupWindow> popup = PopupWindow::Create(
                std::vector<std::string>{ popupText }, {"Close"});
            popup->OpenAndGetButton();
        }
        WindowManager::RedrawAll();

        {
            std::shared_ptr<PopupWindow> popup = PopupWindow::Create(
                std::vector<std::string>{ "Retry?" }, {"Yes", "No"});
            if (popup->OpenAndGetButton() == 1)
                running = false;
        }
        WindowManager::RedrawAll();
    }
}

bool Game::IsRunning() const { return running; }
