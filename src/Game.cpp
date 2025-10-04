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

#define MAX_ENCOUNTERS 5

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

    bool challengeFailed = false;
    for (size_t encounter = 0; encounter < MAX_ENCOUNTERS; encounter++)
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
            std::shared_ptr<PopupWindow> weaponDrop = PopupWindow::Create({
                std::format("Encounter {}: {} won", encounter + 1, player->name),
                std::format("Weapon droped: {}", monster.reward.name),
                std::format("- Damage type: {}", damageType),
                std::format("- Damage:      {}", monster.reward.damage),
                            "Pick it up?"},
                {"No", "Yes"});
            if (weaponDrop->OpenAndGetButton() == 1)
                player->weapon = monster.reward;
        }
            WindowManager::RedrawAll();
            if (player->getTotalLevel() < MAX_LEVEL)
            {
                std::shared_ptr<ClassSelector> selector = ClassSelector::Create(
                    player->lvlRouge, player->lvlWarrior, player->lvlBarbarian);
                player->LevelUp(selector->OpenAndGetClass());
            }
            WindowManager::RedrawAll();
            break;

        case PlayerLost:
        case Draw:
        {
            std::shared_ptr<PopupWindow> windowLost = PopupWindow::Create(
                {log.outcome == Draw ?
                    std::format("Encounter {}:  Draw", encounter + 1) :
                    std::format("Encounter {}: {} won" ,encounter + 1,
                        monster.name),
                "Challange failed. Try again?"},
                {"No", "Yes"});
            if (windowLost->OpenAndGetButton() == 1)
                gameState = CharacterCreation;
            else
                gameState = Stopped;
        }
            challengeFailed = true;
            goto end;

        default: // Do nothing
            break;
        }
    }

end:
    WindowManager::RedrawAll();
    if (!challengeFailed)
    {
        std::string damageType =
            player->weapon.damageType == Slash    ? "Slash"    :
            player->weapon.damageType == Puncture ? "Puncture" :
            player->weapon.damageType == Impact   ? "Impact"   :
                                                    "Unknown"  ;
        std::shared_ptr<PopupWindow> victory = PopupWindow::Create({
                        "Congratulations! Challange cleared",
                        "",
            std::format(" {}", player->name),
            std::format("({})", player->ClassToString()),
            std::format("HP:   {}", player->hp),
                        "",
            std::format("Weapon:   {}", player->weapon.name),
            std::format("- Damage type:   {}", damageType),
            std::format("- Damage:        {}", player->weapon.damage),
                        "",
                        "Stats:",
            std::format("- Strength:    {}", player->str),
            std::format("- Agility:     {}", player->agi),
            std::format("- Endurance:   {}", player->end)},
            {"Exit", "New character"});
        if(victory->OpenAndGetButton() == 0)
            gameState = Stopped;
        else
            gameState = CharacterCreation;
    }
    WindowManager::RedrawAll();
}
