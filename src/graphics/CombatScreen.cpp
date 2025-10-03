#include "graphics/CombatScreen.h"

#include "graphics/WindowManager.h"
#include "skills/Skill.h"
#include "Attack.h"
#include <format>

#define COUNTER_HEIGHT 3
#define COUNTER_WIDTH 12
#define COUNTER_HORIZONTAL_PAD 2

#define STATUS_HEIGHT 16
#define STATUS_WIDTH 37
#define STATUS_VERTICAL_PAD 2
#define STATUS_HORIZONTAL_PAD 3

#define PLAYER_HP_LINE 3
#define MONSTER_HP_LINE 2

#define SKILLS_HEIGH 8
#define SKILLS_WIDTH 56
#define SKILLS_VERTICAL_PAD 1
#define SKILLS_HORIZONTAL_PAD 3

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
        COUNTER_HEIGHT + 1, 0);
    damagePlayer = newwin(DAMAGE_HEIGHT, DAMAGE_WIDTH,
        (STATUS_HEIGHT - DAMAGE_HEIGHT) / 2 + COUNTER_HEIGHT,
        STATUS_WIDTH+DAMAGE_SPACING);

    // Format and fill player skills
    skillsPlayer = newwin(SKILLS_HEIGH, SKILLS_WIDTH,
        COUNTER_HEIGHT + STATUS_HEIGHT + 1, 0);
    for (Skill *skill : log.player->skills)
    {
        skillsTextPlayer.push_back(' ' + skill->name);
        skillsTextPlayer.push_back(skill->description);
    }
    wmove(skillsPlayer, SKILLS_VERTICAL_PAD, 0);
    for (String line : skillsTextPlayer)
        wprintw(skillsPlayer,
            (String(SKILLS_HORIZONTAL_PAD, ' ') + line + '\n').c_str());
    box(skillsPlayer, 0, 0);

    // Format monster status
    statusTextMonster = FormatMonsterStats();
    statusMonster = newwin(STATUS_HEIGHT, STATUS_WIDTH,
        COUNTER_HEIGHT + 1, COLS-STATUS_WIDTH);
    damageMonster = newwin(DAMAGE_HEIGHT, DAMAGE_WIDTH,
        (STATUS_HEIGHT - DAMAGE_HEIGHT) / 2 + COUNTER_HEIGHT,
        COLS-(STATUS_WIDTH+DAMAGE_SPACING+DAMAGE_WIDTH));

    // Format and fill monster skill
    skillsMonster = newwin(SKILLS_HEIGH, SKILLS_WIDTH,
        COUNTER_HEIGHT + STATUS_HEIGHT + 1, COLS - SKILLS_WIDTH);
    skillsTextMonster = {
        log.monster->skill->name, log.monster->skill->description};
    wmove(skillsMonster, SKILLS_VERTICAL_PAD, 0);
    for (String line : skillsTextMonster)
        wprintw(skillsMonster,
            (String(SKILLS_HORIZONTAL_PAD, ' ') + line + '\n').c_str());
    box(skillsMonster, 0, 0);

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
    delwin(skillsPlayer);

    delwin(statusMonster);
    delwin(damageMonster);
    delwin(skillsMonster);

    delwin(controls);
}

void CombatScreen::Open()
{
    // Draw static contents
    refresh();
    wrefresh(skillsPlayer);
    wrefresh(skillsMonster);
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
            // Extra spaces to overwrite larger numbers
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
            // Extra spaces to overwrite larger numbers
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

    mvwin(statusPlayer, COUNTER_HEIGHT + 1, 0);
    mvwin(damagePlayer, (STATUS_HEIGHT - DAMAGE_HEIGHT) / 2 + COUNTER_HEIGHT,
        STATUS_WIDTH+DAMAGE_SPACING);
    mvwin(skillsPlayer, COUNTER_HEIGHT + STATUS_HEIGHT + 1, 0);

    mvwin(statusMonster, COUNTER_HEIGHT + 1, COLS-STATUS_WIDTH);
    mvwin(damageMonster,  (STATUS_HEIGHT - DAMAGE_HEIGHT) / 2 + COUNTER_HEIGHT,
        COLS-(STATUS_WIDTH+DAMAGE_SPACING+DAMAGE_WIDTH));
    mvwin(skillsMonster,
        COUNTER_HEIGHT + STATUS_HEIGHT + 1, COLS - SKILLS_WIDTH);

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
    if (getmaxx(skillsPlayer) != SKILLS_WIDTH ||
        getmaxy(skillsPlayer) != SKILLS_HEIGH)
    {
        wresize(skillsPlayer, SKILLS_HEIGH, SKILLS_WIDTH);
        wmove(skillsPlayer, SKILLS_VERTICAL_PAD, 0);
        for (String line : skillsTextPlayer)
            wprintw(skillsPlayer,
                (String(SKILLS_HORIZONTAL_PAD, ' ') + line + '\n').c_str());
        box(skillsPlayer, 0, 0);
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
    if (getmaxx(skillsMonster) != SKILLS_WIDTH ||
        getmaxy(skillsMonster) != SKILLS_HEIGH)
    {
        wresize(skillsMonster, SKILLS_HEIGH, SKILLS_WIDTH);
        wmove(skillsMonster, SKILLS_VERTICAL_PAD, 0);
        for (String line : skillsTextMonster)
            wprintw(skillsMonster,
                (String(SKILLS_HORIZONTAL_PAD, ' ') + line + '\n').c_str());
        box(skillsMonster, 0, 0);
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
    wrefresh(skillsPlayer);

    wrefresh(statusMonster);
    wrefresh(skillsMonster);

    wrefresh(controls);
}

std::vector<String> CombatScreen::FormatPlayerStats()
{
    std::string damageType =
        log.player->weapon.damageType == Slash    ? "Slash"    :
        log.player->weapon.damageType == Puncture ? "Puncture" :
        log.player->weapon.damageType == Impact   ? "Impact"   :
                                                    "Unknown"  ;

    return std::vector<std::string>{
        std::format(" {}", log.player->name),
        std::format("({})", log.player->ClassToString()),
        std::format("HP:   {}/{}", playerCurrentHP, log.player->hp),
                    "",
        std::format("Weapon:   {}", log.player->weapon.name),
        std::format("- Damage type:   {}", damageType),
        std::format("- Damage:        {}", log.player->weapon.damage),
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
