#include "graphics/ClassSelector.h"

#include "graphics/WindowManager.h"
#include "format"

#define SUBWINDOW_W 28

#define HEIGHT 25
#define WIDTH ((SUBWINDOW_W + 1)*TOTAL_CLASSES + 1)

#define BUTTON_OFFSET 3
#define BUTTON_TEXT " Select   "
#define SELECT_OFFSET 7
#define SELECT_SYMBOL " ↵"

#define CONFIRM_KEY '\n'

std::shared_ptr<ClassSelector> ClassSelector::Create(
    int lvlRouge, int lvlWarrior, int lvlBarbarian)
{
    std::shared_ptr<ClassSelector> ptr(
        new ClassSelector(lvlRouge, lvlWarrior, lvlBarbarian));
    WindowManager::AddWindow(ptr);
    return ptr;
}

ClassSelector::ClassSelector(int lvlRouge, int lvlWarrior, int lvlBarbarian)
{
    // Format outline
    window = newwin(HEIGHT, WIDTH, (LINES - HEIGHT)/2, (COLS - WIDTH)/2);
    for (size_t i = 1; i < TOTAL_CLASSES; i++)
        mvwvline(window, 0, i*(SUBWINDOW_W + 1), 0, HEIGHT);
    box(window, 0, 0);

    // Format and fill rouge section
    rouge = newwin(HEIGHT - 2, SUBWINDOW_W,
        (LINES - HEIGHT)/2 + 1, (COLS - WIDTH)/2 + 1);
    rougeText = (lvlRouge ?
        std::format("\n Rouge - lvl {}\n", lvlRouge) : "\n Rouge\n") +
        "+4 hp per level\n\n"
        "Starting weapon: Dagger\n"
        "- Damage type:   Puncture\n"
        "- Damage:        2\n\n";
    for (size_t i = lvlRouge; i < MAX_LEVEL; i++)
        rougeText += 
            std::format("\n Level {}:\n{}\n", i + 1, rougeBonuses[i]);
    wprintw(rouge, rougeText.c_str());

    // Format and fill warrior section
    warrior = newwin(HEIGHT - 2, SUBWINDOW_W,
        (LINES - HEIGHT)/2 + 1, (COLS - WIDTH)/2 + (SUBWINDOW_W + 1) + 1);
    warriorText = (lvlWarrior ?
        std::format("\n Warrior - lvl {}\n", lvlWarrior) : "\n Warrior\n") +
        "+5 hp per level\n\n"
        "Starting weapon: Sword\n"
        "- Damage type:   Slash\n"
        "- Damage:        3\n\n";
    for (size_t i = lvlWarrior; i < MAX_LEVEL; i++)
        warriorText +=
            std::format("\n Level {}:\n{}\n", i + 1, warriorBonuses[i]);
    wprintw(warrior, warriorText.c_str());

    // Format and fill barbarian section
    barbarian = newwin(HEIGHT - 2, SUBWINDOW_W,
        (LINES - HEIGHT)/2 + 1, (COLS - WIDTH)/2 + (SUBWINDOW_W + 1)*2 + 1);
    barbarianText = (lvlBarbarian ?
        std::format("\n Barbarian - lvl {}\n", lvlBarbarian) : "\n Barbarian\n") +
        "+6 hp per level\n\n"
        "Starting weapon: Club\n"
        "- Damage type:   Impact\n"
        "- Damage:        3\n\n";
    for (size_t i = lvlBarbarian; i < MAX_LEVEL; i++)
        barbarianText +=
            std::format("\n Level {}:\n{}\n", i + 1, barbarianBonuses[i]);
    wprintw(barbarian, barbarianText.c_str());
}

ClassSelector::~ClassSelector()
{
    delwin(window);
    delwin(rouge);
    delwin(warrior);
    delwin(barbarian);
}

PlayerClass ClassSelector::OpenAndGetClass()
{
    keypad(barbarian, TRUE);

    int activeButton = 0;
    bool selectingClass = true;
    while (selectingClass)
    {
        // Redraw buttons
        for (size_t i = 0; i < TOTAL_CLASSES; i++)
        {
            if (i == activeButton)
            {
                wattron(window, A_REVERSE);
                mvwprintw(window, HEIGHT-1, i*(SUBWINDOW_W + 1) + BUTTON_OFFSET,
                    BUTTON_TEXT);
                wmove(window, HEIGHT-1,
                    i*(SUBWINDOW_W + 1) + BUTTON_OFFSET + SELECT_OFFSET);
                wprintw(window, SELECT_SYMBOL);
                wattroff(window, A_REVERSE);
            }
            else
            {
                mvwprintw(window, HEIGHT-1, i*(SUBWINDOW_W + 1) + BUTTON_OFFSET,
                    BUTTON_TEXT);
            }
        }
        
        // Refresh (using side effect of wgetch()) and read input
        wrefresh(window);
        wrefresh(rouge);
        wrefresh(warrior);
        switch(wgetch(barbarian))
        {
        case KEY_LEFT:
            // Rollover Last <- First
            activeButton = (activeButton == 0)
                ? TOTAL_CLASSES - 1 : activeButton - 1;
            break;

        case KEY_RIGHT:
            // Rollover Last -> First
            activeButton = (activeButton + 1) % TOTAL_CLASSES;
            break;

        case CONFIRM_KEY:
            selectingClass = false;
            break;

        case KEY_RESIZE:
            WindowManager::RedrawAll();
            break;

        default:
            break;
        }
    }

    box(window, 0, 0); // Remove buttons
    switch (activeButton)
    {
    case 0:
        return Rouge;
        break;
    case 1:
        return Warrior;
        break;
    case 2:
        return Barbarian;
        break;

    default:
        return NoClass;
        break;
    }
}

void ClassSelector::Redraw()
{
    // Draw separator lines
    for (size_t i = 1; i < TOTAL_CLASSES; i++)
        mvwvline(window, 1, i*(SUBWINDOW_W + 1), 0, HEIGHT - 2);

    // Recenter
    mvwin(window, (LINES - HEIGHT)/2,(COLS - WIDTH)/2);

    mvwin(rouge, (LINES - HEIGHT)/2 + 1,
        (COLS - WIDTH)/2 + 1);
    mvwin(warrior, (LINES - HEIGHT)/2 + 1,
        (COLS - WIDTH)/2 + (SUBWINDOW_W + 1) + 1);
    mvwin(barbarian, (LINES - HEIGHT)/2 + 1,
        (COLS - WIDTH)/2 + (SUBWINDOW_W + 1)*2 + 1);

    // Restore after cut off by small screen
    if (getmaxx(window) != WIDTH || getmaxy(window) != HEIGHT)
    {
        wclear(window);
        wresize(window, HEIGHT, WIDTH);
        box(window, 0, 0);
    }
    if (getmaxx(rouge) != SUBWINDOW_W || getmaxy(rouge) != HEIGHT - 2)
    {
        wclear(rouge);
        wresize(rouge, HEIGHT - 2, SUBWINDOW_W);
        wprintw(rouge, rougeText.c_str());
    }
    if (getmaxx(warrior) != SUBWINDOW_W || getmaxy(warrior) != HEIGHT - 2)
    {
        wclear(warrior);
        wresize(warrior, HEIGHT - 2, SUBWINDOW_W);
        wprintw(warrior, warriorText.c_str());
    }
    if (getmaxx(barbarian) != SUBWINDOW_W || getmaxy(barbarian) != HEIGHT - 2)
    {
        wclear(barbarian);
        wresize(barbarian, HEIGHT - 2, SUBWINDOW_W);
        wprintw(barbarian, barbarianText.c_str());
    }

    // Draw
    wrefresh(window);
    wrefresh(rouge);
    wrefresh(warrior);
    wrefresh(barbarian);
}
