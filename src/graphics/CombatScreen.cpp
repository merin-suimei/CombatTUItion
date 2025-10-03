#include "graphics/CombatScreen.h"

#include "graphics/WindowManager.h"
#include "skills/Skill.h"
#include "Attack.h"
#include <format>

#define DEFAULT_HPAD 3

#define COUNTER_H 3
#define COUNTER_W 12
#define COUNTER_HPAD 2

#define STATUS_H 16
#define STATUS_W 37
#define STATUS_VPAD 2

#define PLAYER_HP_LINE 3
#define MONSTER_HP_LINE 2

#define SKILLS_H 8
#define SKILLS_W 56
#define SKILLS_VPAD 1

#define DAMAGE_SPACING 3
#define DAMAGE_H 2
#define DAMAGE_W 7

#define CONTROLS_H 3
#define CONTROLS_HPAD 2
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
    mvhline(COUNTER_H - 1, 0, 0, COLS);
    mvvline(0, COLS/2, 0, LINES);

    // Format turn counter
    counter = newwin(COUNTER_H, COUNTER_W, 0, (COLS - COUNTER_W)/2);
    box(counter, 0, 0);

    // Format player status
    statusTextPlayer = FormatPlayerStats();
    statusPlayer = newwin(STATUS_H, STATUS_W, COUNTER_H + 1, 0);
    damagePlayer = newwin(DAMAGE_H, DAMAGE_W,
        (STATUS_H - DAMAGE_H)/2 + COUNTER_H, STATUS_W + DAMAGE_SPACING);

    // Format and fill player skills
    skillsPlayer = newwin(SKILLS_H, SKILLS_W, COUNTER_H + STATUS_H + 1, 0);
    for (Skill *skill : log.player->skills)
    {
        skillsTextPlayer.push_back(' ' + skill->name);
        skillsTextPlayer.push_back(skill->description);
    }
    PrintMultiline(skillsPlayer, SKILLS_VPAD, DEFAULT_HPAD, skillsTextPlayer);

    // Format monster status
    statusTextMonster = FormatMonsterStats();
    statusMonster = newwin(STATUS_H, STATUS_W, COUNTER_H + 1, COLS - STATUS_W);
    damageMonster = newwin(DAMAGE_H, DAMAGE_W,
        (STATUS_H - DAMAGE_H)/2 + COUNTER_H,
        COLS - (STATUS_W + DAMAGE_SPACING + DAMAGE_W));

    // Format and fill monster skill
    skillsMonster = newwin(SKILLS_H, SKILLS_W,
        COUNTER_H + STATUS_H + 1, COLS - SKILLS_W);
    skillsTextMonster = {
        log.monster->skill->name, log.monster->skill->description};
    PrintMultiline(skillsMonster, SKILLS_VPAD, DEFAULT_HPAD, skillsTextMonster);

    // Format and fill controls tip
    controlsText = CONTROL_TIP;
    controlsWidth = controlsText.size() + CONTROLS_HPAD*2;
    controls = newwin(CONTROLS_H, controlsWidth,
        LINES-CONTROLS_H, (COLS - controlsWidth)/2);
    mvwprintw(controls, 1, CONTROLS_HPAD, controlsText.c_str());
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
    PrintMultiline(statusPlayer, STATUS_VPAD, DEFAULT_HPAD, statusTextPlayer);
    wrefresh(statusPlayer);

    // Draw monster's initial stats
    PrintMultiline(statusMonster, STATUS_VPAD, DEFAULT_HPAD, statusTextMonster);
    wrefresh(statusMonster);

    Attack *playerAttack;
    Attack *monsterAttack;
    // Draw dynamic contents
    for (size_t currentTurn = 1; currentTurn <= log.turn; currentTurn++)
    {
        // Draw turn counter
        counterText = std::format("Turn: {}", currentTurn).c_str();
        mvwprintw(counter, 1, COUNTER_HPAD, counterText.c_str());
        wrefresh(counter);

        // Player's halfturn
        playerAttack = &log.attacks.at((currentTurn-1)*2);
        monsterCurrentHP -= playerAttack->dmg;

        mvwprintw(statusMonster, STATUS_VPAD + MONSTER_HP_LINE - 1, DEFAULT_HPAD,
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
        if ((currentTurn-1)*2 + 1 >= log.attacks.size())
            break;

        // Monster's halfturn
        monsterAttack = &log.attacks.at((currentTurn - 1)*2 + 1);
        playerCurrentHP -= monsterAttack->dmg;

        mvwprintw(statusPlayer, STATUS_VPAD + PLAYER_HP_LINE - 1, DEFAULT_HPAD,
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
    mvhline(COUNTER_H - 1, 0, 0, COLS);
    mvvline(0, COLS/2, 0, LINES);
    refresh();

    // Recenter
    mvwin(counter, 0, (COLS - COUNTER_W)/2);

    mvwin(statusPlayer, COUNTER_H + 1, 0);
    mvwin(damagePlayer, (STATUS_H - DAMAGE_H) / 2 + COUNTER_H,
        STATUS_W + DAMAGE_SPACING);
    mvwin(skillsPlayer, COUNTER_H + STATUS_H + 1, 0);

    mvwin(statusMonster, COUNTER_H + 1, COLS - STATUS_W);
    mvwin(damageMonster,  (STATUS_H - DAMAGE_H)/2 + COUNTER_H,
        COLS-(STATUS_W+DAMAGE_SPACING+DAMAGE_W));
    mvwin(skillsMonster, COUNTER_H + STATUS_H + 1, COLS - SKILLS_W);

    mvwin(controls, LINES-CONTROLS_H, (COLS - controlsText.size())/2);

    // Restore after cut off by small screen
    if (getmaxx(counter) != COUNTER_W || getmaxy(counter) != COUNTER_H)
    {
        wresize(counter, COUNTER_H, COUNTER_W);
        mvwprintw(counter, 1, COUNTER_HPAD, counterText.c_str());
        box(counter, 0, 0);
    }

    if (getmaxx(statusPlayer) != STATUS_W || getmaxy(statusPlayer) != STATUS_H)
    {
        wresize(statusPlayer, STATUS_H, STATUS_W);
        PrintMultiline(statusPlayer, STATUS_VPAD, DEFAULT_HPAD,
            statusTextPlayer);
    }
    if (getmaxx(damagePlayer) != DAMAGE_W || getmaxy(damagePlayer) != DAMAGE_H)
        wresize(damagePlayer, DAMAGE_H, DAMAGE_W);

    if (getmaxx(skillsPlayer) != SKILLS_W || getmaxy(skillsPlayer) != SKILLS_H)
    {
        wresize(skillsPlayer, SKILLS_H, SKILLS_W);
        PrintMultiline(skillsPlayer, SKILLS_VPAD, DEFAULT_HPAD,
            skillsTextPlayer);
    }

    if (getmaxx(statusMonster) != STATUS_W || getmaxy(statusMonster) != STATUS_H)
    {
        wresize(statusMonster, STATUS_H, STATUS_W);
        PrintMultiline(statusMonster, STATUS_VPAD, DEFAULT_HPAD,
            statusTextMonster);
    }
    if (getmaxx(damageMonster) != DAMAGE_W || getmaxy(damageMonster) != DAMAGE_H)
        wresize(damageMonster, DAMAGE_H, DAMAGE_W);

    if (getmaxx(skillsMonster) != SKILLS_W || getmaxy(skillsMonster) != SKILLS_H)
    {
        wresize(skillsMonster, SKILLS_H, SKILLS_W);
        PrintMultiline(skillsMonster, SKILLS_VPAD, DEFAULT_HPAD,
            skillsTextMonster);
    }

    if (getmaxx(controls) != controlsWidth || getmaxy(controls) != CONTROLS_H)
    {
        wresize(controls, CONTROLS_H, controlsWidth);
        mvwprintw(controls, 1, CONTROLS_HPAD, controlsText.c_str());
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

void CombatScreen::PrintMultiline(WINDOW *win, int vpad, int hpad,
    std::vector<String> text)
{
    wmove(win, vpad, 0);
    for (String line : text)
        wprintw(win, (String(hpad, ' ') + line + '\n').c_str());
    box(win, 0, 0);
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
