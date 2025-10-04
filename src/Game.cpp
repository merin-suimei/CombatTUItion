#include "Game.h"

#include "entities/Monster.h"
#include "graphics/ClassSelector.h"
#include "graphics/CombatScreen.h"
#include "graphics/PopupWindow.h"
#include "graphics/WindowManager.h"
#include "Combat.h"
#include <curses.h>
#include <format>
#include <random>
#include <string>

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
    gameState = CharacterCreation;

    while (gameState != Stopped)
    {
        switch (gameState)
        {
        case CharacterCreation:
            if (player != nullptr)
                delete player;
            {
                std::shared_ptr<ClassSelector> window =
                    ClassSelector::Create(0, 0, 0);
                player = new Player("Nameless", window->OpenAndGetClass());
            }
            WindowManager::RedrawAll();
            gameState = Challenge;
            // Fall-through

        case Challenge:
            RunChallenge();
            break;

        default:
            gameState = Stopped;
            break;
        }
    }
}

GameState Game::getGameState() const
{
    return gameState;
}

void Game::RunChallenge()
{
    // Prepare random
    std::random_device rd;
    std::mt19937 gen(rd());

    // Hardcoded (First, Last)
    std::uniform_int_distribution<> monsterDist(Goblin, Dragon);

    {
        Monster monster = monstersTable.at(MonsterType(monsterDist(gen)));
        EncounterLog log = Combat::simulateEncounter(player, &monster);

        std::shared_ptr<CombatScreen> window = CombatScreen::Create(log);
        window->Open();

        switch (log.outcome)
        {
        case PlayerWon:
        {
            std::string damageType =
                monster.reward.damageType == Slash    ? "Slash"    :
                monster.reward.damageType == Puncture ? "Puncture" :
                monster.reward.damageType == Impact   ? "Impact"   :
                                                        "Unknown"  ;
            std::shared_ptr<PopupWindow> popup = PopupWindow::Create({
                std::format(" {} won", player->name),
                std::format("Weapon droped: {}", monster.reward.name),
                std::format("- Damage type: {}", damageType),
                std::format("- Damage:      {}", monster.reward.damage),
                            "Pick it up?"},
                {"No", "Yes"});
            if (popup->OpenAndGetButton() == 1)
                player->weapon = monster.reward;
        }
            WindowManager::RedrawAll();
            if (player->getTotalLevel() < MAX_LEVEL)
            {
                std::shared_ptr<ClassSelector> selector = ClassSelector::Create(
                    player->lvlRouge, player->lvlWarrior, player->lvlBarbarian);
                player->LevelUp(selector->OpenAndGetClass());
            }
            break;

        case PlayerLost:
        case Draw:
        {
            std::shared_ptr<PopupWindow> popup = PopupWindow::Create(
                {log.outcome == Draw ?
                    " Draw" : std::format(" {} won", monster.name),
                "Challange failed. Try again?"},
                {"No", "Yes"});
            if (popup->OpenAndGetButton() == 1)
                gameState = CharacterCreation;
            else
                gameState = Stopped;
        }
            break;

        default: // Do nothing
            break;
        }
    }
    WindowManager::RedrawAll();
}
