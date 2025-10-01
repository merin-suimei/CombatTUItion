#include "graphics/CombatScreen.h"

#include "graphics/WindowManager.h"
#include <format>

#define COUNTER_HEIGHT 3
#define COUNTER_WIDTH 12
#define COUNTER_HORIZONTAL_PAD 2

#define STATUS_HEIGHT 12
#define STATUS_WIDTH 30
#define STATUS_VERTICAL_PAD 2
#define STATUS_HORIZONTAL_PAD 3

#define PLAYER_HP_LINE 2
#define MONSTER_HP_LINE 2

#define DAMAGE_SPACING 3
#define DAMAGE_HEIGHT 2
#define DAMAGE_WIDTH 7

#define CONTROLS_HEIGHT 3
#define CONTROLS_HORIZONTAL_PAD 2
#define CONTROL_TIP "Press any key to speed up turn"

#define HALFTURN_DELAY_MS 1000
#define TURN_DELAY_MS 1500

using String = std::string;

std::shared_ptr<CombatScreen> CombatScreen::Create(const EncounterLog log)
{
    std::shared_ptr<CombatScreen> ptr(new CombatScreen(log));
    WindowManager::AddWindow(ptr);
    return ptr;
}

CombatScreen::CombatScreen(const EncounterLog log)
{
    this->log = log;
    playerCurrentHP = log.player->hp;
    monsterCurrentHP = log.monster->hp;

    // Prepare separator lines
    mvhline(COUNTER_HEIGHT-1, 0, 0, COLS);
    mvvline(0, COLS / 2, 0, LINES);

    // Format turn counter
    counter = newwin(COUNTER_HEIGHT, COUNTER_WIDTH,
        0, (COLS - COUNTER_WIDTH) / 2);
    box(counter, 0, 0);

    // Format player status
    statusTextPlayer = FormatPlayerStats();
    statusPlayer = newwin(STATUS_HEIGHT, STATUS_WIDTH,
        (LINES - STATUS_HEIGHT) / 2, 0);
    damagePlayer = newwin(DAMAGE_HEIGHT, DAMAGE_WIDTH,
        (LINES - DAMAGE_HEIGHT) / 2, STATUS_WIDTH+DAMAGE_SPACING);

    // Format monster status
    statusTextMonster = FormatMonsterStats();
    statusMonster = newwin(STATUS_HEIGHT, STATUS_WIDTH,
        (LINES - STATUS_HEIGHT) / 2, COLS-STATUS_WIDTH);
    damageMonster = newwin(DAMAGE_HEIGHT, DAMAGE_WIDTH,
        (LINES - DAMAGE_HEIGHT) / 2,
        COLS-(STATUS_WIDTH+DAMAGE_SPACING+DAMAGE_WIDTH));

    // Format and fill controls tip
    controlsText = CONTROL_TIP;
    controlsWidth = controlsText.size() + CONTROLS_HORIZONTAL_PAD*2;
    controls = newwin(CONTROLS_HEIGHT, controlsWidth,
        LINES-CONTROLS_HEIGHT, (COLS - controlsWidth) / 2);
    mvwprintw(controls, 1, CONTROLS_HORIZONTAL_PAD, controlsText.c_str());
    box(controls, 0, 0);
}

CombatScreen::~CombatScreen()
{
    delwin(counter);

    delwin(statusPlayer);
    delwin(damagePlayer);

    delwin(statusMonster);
    delwin(damageMonster);

    delwin(controls);
}

void CombatScreen::Open()
{
    // Draw static contents
    refresh();
    wrefresh(controls);

    // Draw player's initial stats
    wmove(statusPlayer, STATUS_VERTICAL_PAD, 0);
    for (String line : statusTextPlayer)
        wprintw(statusPlayer,
            (String(STATUS_HORIZONTAL_PAD, ' ') + line + '\n').c_str());
    box(statusPlayer, 0, 0);
    wrefresh(statusPlayer);

    // Draw monster's initial stats
    wmove(statusMonster, STATUS_VERTICAL_PAD, 0);
    for (String line : statusTextMonster)
        wprintw(statusMonster,
            (String(STATUS_HORIZONTAL_PAD, ' ') + line + '\n').c_str());
    box(statusMonster, 0, 0);
    wrefresh(statusMonster);

    Attack *playerAttack;
    Attack *monsterAttack;
    // Draw dynamic contents
    for (size_t currentTurn = 1; currentTurn <= log.turn; currentTurn++)
    {
        // Draw turn counter
        counterText = std::format("Turn: {}", currentTurn).c_str();
        mvwprintw(counter, 1, COUNTER_HORIZONTAL_PAD, counterText.c_str());
        wrefresh(counter);

        // Player's halfturn
        playerAttack = &log.attacks.at((currentTurn-1)*2);
        monsterCurrentHP -= playerAttack->dmg;

        mvwprintw(statusMonster,
            STATUS_VERTICAL_PAD + MONSTER_HP_LINE - 1, STATUS_HORIZONTAL_PAD,
            // Extra spaces to overwrive larger numbers
            std::format("HP:       {}/{}   ",
                monsterCurrentHP, log.monster->hp).c_str());
        box(statusMonster, 0, 0);
        wrefresh(statusMonster);

        mvwprintw(damageMonster, 0, 0, std::format("{}",
            playerAttack->flag != Miss ? std::to_string(-playerAttack->dmg) :
                                        "Miss").c_str());
        wrefresh(damageMonster);

        wtimeout(controls, HALFTURN_DELAY_MS);
        if(wgetch(controls) == KEY_RESIZE)
            WindowManager::RedrawAll();
        wclear(damageMonster);
        wrefresh(damageMonster);

        // Check if next halfturn exists
        if ((currentTurn-1)*2+1 >= log.attacks.size())
            break;

        // Monster's halfturn
        monsterAttack = &log.attacks.at((currentTurn-1)*2+1);
        playerCurrentHP -= monsterAttack->dmg;

        mvwprintw(statusPlayer,
            STATUS_VERTICAL_PAD + PLAYER_HP_LINE - 1, STATUS_HORIZONTAL_PAD,
            // Extra spaces to overwrive larger numbers
            std::format("HP:   {}/{}   ",
                playerCurrentHP, log.player->hp).c_str());
        box(statusPlayer, 0, 0);
        wrefresh(statusPlayer);

        mvwprintw(damagePlayer, 0, 0, std::format("{}",
            monsterAttack->flag != Miss ? std::to_string(-monsterAttack->dmg) :
                                        "Miss").c_str());
        wrefresh(damagePlayer);

        wtimeout(controls, TURN_DELAY_MS);
        if(wgetch(controls) == KEY_RESIZE)
            WindowManager::RedrawAll();
        wclear(damagePlayer);
        wrefresh(damagePlayer);
    }
}

void CombatScreen::Redraw()
{
    // Draw separator lines
    mvhline(COUNTER_HEIGHT-1, 0, 0, COLS);
    mvvline(0, COLS / 2, 0, LINES);
    refresh();

    // Recenter
    mvwin(counter, 0, (COLS - COUNTER_WIDTH) / 2);

    mvwin(statusPlayer, (LINES - STATUS_HEIGHT) / 2, 0);
    mvwin(damagePlayer, (LINES - DAMAGE_HEIGHT) / 2,
        STATUS_WIDTH+DAMAGE_SPACING);

    mvwin(statusMonster, (LINES - STATUS_HEIGHT) / 2, COLS-STATUS_WIDTH);
    mvwin(damageMonster, (LINES - DAMAGE_HEIGHT) / 2,
        COLS-(STATUS_WIDTH+DAMAGE_SPACING+DAMAGE_WIDTH));

    mvwin(controls, LINES-CONTROLS_HEIGHT, (COLS - controlsText.size()) / 2);

    // Restore after cut off by small screen
    if (getmaxx(counter) != COUNTER_WIDTH ||
        getmaxy(counter) != COUNTER_HEIGHT)
    {
        wresize(counter, COUNTER_HEIGHT, COUNTER_WIDTH);
        mvwprintw(counter, 1, COUNTER_HORIZONTAL_PAD, counterText.c_str());
        box(counter, 0, 0);
    }

    if (getmaxx(statusPlayer) != STATUS_WIDTH ||
        getmaxy(statusPlayer) != STATUS_HEIGHT)
    {
        wresize(statusPlayer, STATUS_HEIGHT, STATUS_WIDTH);
        wmove(statusPlayer, STATUS_VERTICAL_PAD, 0);
        for (String line : statusTextPlayer)
            wprintw(statusPlayer,
                (String(STATUS_HORIZONTAL_PAD, ' ') + line + '\n').c_str());
        box(statusPlayer, 0, 0);
    }
    if (getmaxx(damagePlayer) != DAMAGE_WIDTH ||
        getmaxy(damagePlayer) != DAMAGE_HEIGHT)
    {
        wresize(damagePlayer, DAMAGE_HEIGHT, DAMAGE_WIDTH);
    }

    if (getmaxx(statusMonster) != STATUS_WIDTH ||
        getmaxy(statusMonster) != STATUS_HEIGHT)
    {
        wresize(statusMonster, STATUS_HEIGHT, STATUS_WIDTH);
        wmove(statusMonster, STATUS_VERTICAL_PAD, 0);
        for (String line : statusTextMonster)
            wprintw(statusMonster,
                (String(STATUS_HORIZONTAL_PAD, ' ') + line + '\n').c_str());
        box(statusMonster, 0, 0);
    }
    if (getmaxx(damageMonster) != DAMAGE_WIDTH ||
        getmaxy(damageMonster) != DAMAGE_HEIGHT)
    {
        wresize(damageMonster, DAMAGE_HEIGHT, DAMAGE_WIDTH);
    }

    if (getmaxx(controls) != controlsWidth ||
        getmaxy(controls) != CONTROLS_HEIGHT)
    {
        wresize(controls, CONTROLS_HEIGHT, controlsWidth);
        mvwprintw(controls, 1, CONTROLS_HORIZONTAL_PAD, controlsText.c_str());
        box(controls, 0, 0);
    }

    // Draw everything
    wrefresh(counter);
    wrefresh(statusPlayer);
    wrefresh(statusMonster);
    wrefresh(controls);
}

std::vector<String> CombatScreen::FormatPlayerStats()
{
    return std::vector<std::string>{
        std::format(" {} ({})", log.player->name, log.player->ClassToString()),
        std::format("HP:   {}/{}", playerCurrentHP, log.player->hp),
                    "",
                    "Stats:",
        std::format("- Strength:    {}", log.player->str),
        std::format("- Agility:     {}", log.player->agi),
        std::format("- Endurance:   {}", log.player->end)
    };
}

std::vector<String> CombatScreen::FormatMonsterStats()
{
    return std::vector<std::string>{
        std::format(" {}", log.monster->name),
        std::format("HP:       {}/{}", monsterCurrentHP, log.monster->hp),
        std::format("Damage:   {}", log.monster->dmg),
                    "",
                    "Stats:",
        std::format("- Strength:    {}", log.monster->str),
        std::format("- Agility:     {}", log.monster->agi),
        std::format("- Endurance:   {}", log.monster->end)
    };
}
