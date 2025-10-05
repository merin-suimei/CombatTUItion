#include "graphics/PopupWindow.h"

#include "graphics/WindowManager.h"

#define HORIZONTAL_PAD 3
#define VERTICAL_PAD 2

#define BUTTON_LPAD 1
#define BUTTON_RPAD 3
#define SELECT_SYMBOL " ↵"
#define BUTTON_SPACING 2

#define CONFIRM_KEY '\n'

using String = std::string;

std::shared_ptr<PopupWindow> PopupWindow::Create(
    const std::vector<String> text,
    const std::vector<String> buttons)
{
    std::shared_ptr<PopupWindow> ptr(new PopupWindow(text, buttons));
    WindowManager::AddWindow(ptr);
    return ptr;
}

PopupWindow::PopupWindow(
    const std::vector<String> text,
    const std::vector<String> buttons)
{
    for (String line : text)
        this->text += String(HORIZONTAL_PAD, ' ') + line + '\n';

    // Format buttons
    for (String button : buttons)
        this->buttons.push_back(
            String(BUTTON_LPAD, ' ') + button + String(BUTTON_RPAD, ' '));

    // Set height
    height = text.size() + VERTICAL_PAD*2;

    // Set width
    width = (buttons.size()-1) * BUTTON_SPACING;
    for (String button : buttons)
        width += button.size() + BUTTON_LPAD + BUTTON_RPAD;
    for (String line : text)
        if (line.size() > width)
            width = line.size();
    width += HORIZONTAL_PAD*2;

    // Fill button position offsets
    buttonsOffset.push_back(HORIZONTAL_PAD);
    for (size_t i = 1; i < buttons.size(); i++)
        buttonsOffset.push_back(buttonsOffset[i-1] + buttons[i-1].size()
            + BUTTON_SPACING + BUTTON_LPAD + BUTTON_RPAD);

    // Format and fill window
    window = newwin(height, width,
        (LINES - height) / 2,
        (COLS - width) / 2);
    mvwprintw(window, VERTICAL_PAD, 0, this->text.c_str());
    box(window, 0, 0);
}

PopupWindow::~PopupWindow()
{
    delwin(window);
}

// Returns index of pressed button
int PopupWindow::OpenAndGetButton()
{
    keypad(window, TRUE);

    int activeButton = 0;
    bool selectingButton = true;
    while (selectingButton)
    {
        // Redraw buttons
        for (size_t i = 0; i < buttons.size(); i++)
        {
            if (i == activeButton)
            {
                wattron(window, A_REVERSE);
                mvwprintw(window, height-1, buttonsOffset[i],
                    buttons[i].c_str());
                wmove(window, height-1,
                    buttonsOffset[i] + buttons[i].size()-BUTTON_RPAD);
                wprintw(window, SELECT_SYMBOL);
                wattroff(window, A_REVERSE);
            }
            else
            {
                mvwprintw(window, height-1, buttonsOffset[i],
                    buttons[i].c_str());
            }
        }

        // Refresh (using side effect of wgetch()) and read input
        switch(wgetch(window))
        {
        case KEY_LEFT:
            // Rollover Last <- First
            activeButton = (activeButton == 0)
                ? buttons.size()-1 : activeButton-1;
            break;

        case KEY_RIGHT:
            // Rollover Last -> First
            activeButton = (activeButton + 1) % buttons.size();
            break;

        case CONFIRM_KEY:
            selectingButton = false;
            break;

        case KEY_RESIZE:
            WindowManager::RedrawAll();
            break;

        default:
            break;
        }
    }

    box(window, 0, 0); // Remove buttons
    return activeButton;
}

void PopupWindow::Redraw()
{
    // Recenter
    mvwin(window,
        (LINES - height) / 2,
        (COLS - width) / 2);

    // Restore after cut off by small screen
    if (getmaxx(window) != width ||
        getmaxy(window) != height)
    {
        wresize(window, height, width);
        mvwprintw(window, VERTICAL_PAD, 0, this->text.c_str());
        box(window, 0, 0);
    }

    // Draw
    wrefresh(window);
}
