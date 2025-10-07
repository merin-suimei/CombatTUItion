#include "graphics/CombatScreen.h"

#include "graphics/WindowManager.h"
#include "skills/Skill.h"
#include <format>

#define HPAD_SIZE 3
#define HPAD std::string(HPAD_SIZE, ' ')

#define COUNTER_H 3
#define COUNTER_W 12
#define COUNTER_HPAD 2

#define STATUS_H 16
#define STATUS_W 37
#define STATUS_VPAD 2

#define PLAYER_HP_LINE 3
#define MONSTER_HP_LINE 2

#define SKILLS_H 8
#define SKILLS_W 61
#define SKILLS_VPAD 1

#define DAMAGE_SPACING 3
#define DAMAGE_H 2
#define DAMAGE_W 7

#define CONTROLS_H 3
#define CONTROLS_HPAD 2
#define CONTROL_TIP "Press or hold any key to speed up turn"

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

    // Format and fill player status
    statusTextPlayer = FormatPlayerStats();
    statusPlayer = newwin(STATUS_H, STATUS_W, COUNTER_H + 1, 0);
    damagePlayer = newwin(DAMAGE_H, DAMAGE_W,
        (STATUS_H - DAMAGE_H)/2 + COUNTER_H, STATUS_W + DAMAGE_SPACING);
    mvwprintw(statusPlayer, STATUS_VPAD, 0, statusTextPlayer.c_str());
    box(statusPlayer, 0, 0);

    // Format and fill player skills
    skillsPlayer = newwin(SKILLS_H, SKILLS_W, COUNTER_H + STATUS_H + 1, 0);
    for (size_t i = 0; i < log.player->skillCount; i++)
        skillsTextPlayer += std::format("{}{}\n{}{}\n",
            std::string(HPAD_SIZE + 1, ' '), log.player->skills[i]->name,
            HPAD, log.player->skills[i]->description);
    mvwprintw(skillsPlayer, SKILLS_VPAD, 0, skillsTextPlayer.c_str());
    box(skillsPlayer, 0, 0);

    // Format and fill monster status
    statusTextMonster = FormatMonsterStats();
    statusMonster = newwin(STATUS_H, STATUS_W, COUNTER_H + 1, COLS - STATUS_W);
    damageMonster = newwin(DAMAGE_H, DAMAGE_W,
        (STATUS_H - DAMAGE_H)/2 + COUNTER_H,
        COLS - (STATUS_W + DAMAGE_SPACING + DAMAGE_W));
    mvwprintw(statusMonster, STATUS_VPAD, 0, statusTextMonster.c_str());
    box(statusMonster, 0, 0);

    // Format and fill monster skill
    skillsMonster = newwin(SKILLS_H, SKILLS_W,
        COUNTER_H + STATUS_H + 1, COLS - SKILLS_W);
    skillsTextMonster = std::format("{}{}\n{}{}\n",
        std::string(HPAD_SIZE + 1, ' '), log.monster->skill->name,
        HPAD, log.monster->skill->description);
    mvwprintw(skillsMonster, SKILLS_VPAD, 0, skillsTextMonster.c_str());
    box(skillsMonster, 0, 0);

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
    wrefresh(statusPlayer);
    wrefresh(skillsPlayer);

    wrefresh(statusMonster);
    wrefresh(skillsMonster);
    wrefresh(controls);

    // Draw dynamic contents
    for (size_t currentTurn = 1; currentTurn <= log.turn; currentTurn++)
    {
        // Draw turn counter
        counterText = std::format("Turn: {}", currentTurn);
        mvwprintw(counter, 1, COUNTER_HPAD, counterText.c_str());
        wrefresh(counter);

        if (log.initiative == PlayerFirst)
            DrawPlayerHalfturn((currentTurn - 1)*2);
        else
            DrawMonsterHalfturn((currentTurn - 1)*2);

        wtimeout(controls, HALFTURN_DELAY_MS);
        if(wgetch(controls) == KEY_RESIZE)
            WindowManager::RedrawAll();
        wrefresh(damagePlayer);
        wrefresh(damageMonster);

        // Check if next halfturn exists
        if ((currentTurn - 1)*2 + 1 >= log.attacks.size())
            break;

        if (log.initiative == MonsterFirst)
            DrawPlayerHalfturn((currentTurn - 1)*2 + 1);
        else
            DrawMonsterHalfturn((currentTurn - 1)*2 + 1);

        wtimeout(controls, TURN_DELAY_MS);
        if(wgetch(controls) == KEY_RESIZE)
            WindowManager::RedrawAll();
        wrefresh(damagePlayer);
        wrefresh(damageMonster);
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
    mvwin(damageMonster, (STATUS_H - DAMAGE_H)/2 + COUNTER_H,
        COLS - (STATUS_W + DAMAGE_SPACING + DAMAGE_W));
    mvwin(skillsMonster, COUNTER_H + STATUS_H + 1, COLS - SKILLS_W);

    mvwin(controls, LINES - CONTROLS_H, (COLS - controlsWidth)/2);

    // Restore after cut off by small screen
    RestoreWindow(counter, COUNTER_H, COUNTER_W,
        1, COUNTER_HPAD, counterText.c_str());

    RestoreWindow(statusPlayer, STATUS_H, STATUS_W,
        STATUS_VPAD, 0, statusTextPlayer.c_str());
    if (getmaxx(damagePlayer) != DAMAGE_W || getmaxy(damagePlayer) != DAMAGE_H)
        wresize(damagePlayer, DAMAGE_H, DAMAGE_W);
    RestoreWindow(skillsPlayer, SKILLS_H, SKILLS_W,
        SKILLS_VPAD, 0, skillsTextPlayer.c_str());

    RestoreWindow(statusMonster, STATUS_H, STATUS_W,
        STATUS_VPAD, 0, statusTextMonster.c_str());
    if (getmaxx(damageMonster) != DAMAGE_W || getmaxy(damageMonster) != DAMAGE_H)
        wresize(damageMonster, DAMAGE_H, DAMAGE_W);
    RestoreWindow(skillsMonster, SKILLS_H, SKILLS_W,
        SKILLS_VPAD, 0, skillsTextMonster.c_str());

    RestoreWindow(controls, CONTROLS_H, controlsWidth,
        1, CONTROLS_HPAD, controlsText.c_str());

    // Draw everything
    wrefresh(counter);

    wrefresh(statusPlayer);
    wrefresh(skillsPlayer);

    wrefresh(statusMonster);
    wrefresh(skillsMonster);

    wrefresh(controls);
}

void CombatScreen::RestoreWindow(WINDOW *window, int height, int width,
    int textY, int textX, const char *text)
{
    if (getmaxy(window) != height || getmaxx(window) != width)
    {
        wresize(window, height, width);
        mvwprintw(window, textY, textX, text);
        box(window, 0, 0);
    }
}

void CombatScreen::DrawPlayerHalfturn(size_t currentAttack)
{
    // Player's halfturn
    Attack *playerAttack = &log.attacks.at(currentAttack);
    monsterCurrentHP -= playerAttack->dmg;

    mvwprintw(statusMonster, STATUS_VPAD + MONSTER_HP_LINE - 1, HPAD_SIZE,
        // Extra spaces to overwrite larger numbers
        std::format("HP:       {}/{}   ", monsterCurrentHP, log.monster->hp).c_str());
    box(statusMonster, 0, 0);
    wrefresh(statusMonster);

    mvwprintw(damageMonster, 0, 0, std::format("{}",
        playerAttack->flag != Miss ? std::to_string(-playerAttack->dmg) :
                                     "Miss").c_str());
    wrefresh(damageMonster);
    wclear(damageMonster);
}


void CombatScreen::DrawMonsterHalfturn(size_t currentAttack)
{
    // Monster's halfturn
    Attack *monsterAttack = &log.attacks.at(currentAttack);
    playerCurrentHP -= monsterAttack->dmg;

    mvwprintw(statusPlayer, STATUS_VPAD + PLAYER_HP_LINE - 1, HPAD_SIZE,
        // Extra spaces to overwrite larger numbers
        std::format("HP:   {}/{}   ", playerCurrentHP, log.player->hp).c_str());
    box(statusPlayer, 0, 0);
    wrefresh(statusPlayer);

    mvwprintw(damagePlayer, 0, 0, std::format("{}",
        monsterAttack->flag != Miss ? std::to_string(-monsterAttack->dmg) :
                                      "Miss").c_str());
    wrefresh(damagePlayer);
    wclear(damagePlayer);
}

String CombatScreen::FormatPlayerStats()
{
    const Player *player = log.player;
    std::string damageType =
        player->weapon.damageType == Slash    ? "Slash"    :
        player->weapon.damageType == Puncture ? "Puncture" :
        player->weapon.damageType == Impact   ? "Impact"   :
                                                "Unknown"  ;

    return std::format(
        "{}{}\n"
        "{}({})\n"
        "{}HP:   {}/{}\n\n"
        "{}Weapon:   {}\n"
        "{}- Damage type:   {}\n"
        "{}- Damage:        {}\n\n"
        "{}Stats:\n"
        "{}- Strength:    {}\n"
        "{}- Agility:     {}\n"
        "{}- Endurance:   {}",
        std::string(HPAD_SIZE + 1, ' '), player->name,
        HPAD, player->ClassToString(), HPAD, playerCurrentHP, player->hp,
        HPAD, player->weapon.name,
        HPAD, damageType, HPAD, player->weapon.damage,
        HPAD, HPAD, player->str, HPAD, player->agi, HPAD, player->end);
}

String CombatScreen::FormatMonsterStats()
{
    const Monster *monster = log.monster;
    return std::format(
        "{}{}\n"
        "{}HP:       {}/{}\n"
        "{}Damage:   {}\n\n"
        "{}Stats:\n"
        "{}- Strength:    {}\n"
        "{}- Agility:     {}\n"
        "{}- Endurance:   {}\n",
        std::string(HPAD_SIZE + 1, ' '), monster->name,
        HPAD, monsterCurrentHP, monster->hp, HPAD, monster->dmg,
        HPAD, HPAD, monster->str, HPAD, monster->agi, HPAD,  monster->end);
}
