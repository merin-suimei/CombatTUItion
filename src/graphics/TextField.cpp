#include "graphics/TextField.h"

#include "graphics/WindowManager.h"
#include <format>

#define HEIGHT 4
#define WIDTH (INPUT_LEN + HPAD*2)

#define INPUT_LEN 32
#define HPAD 3

std::shared_ptr<TextField> TextField::Create(const std::string &text)
{
    std::shared_ptr<TextField> ptr(new TextField(text));
    WindowManager::AddWindow(ptr);
    return ptr;
}

TextField::TextField(const std::string &text)
{
    this->text = text;

    // Set input visability
    echo();
    curs_set(1);

    // Format and fill window
    window = newwin(HEIGHT, WIDTH, (LINES - HEIGHT)/2, (COLS - WIDTH)/2);
    mvwprintw(window, 1, HPAD,
        std::format("{}\n{}>", text, std::string(HPAD - 1, ' ')).c_str());
    box(window, 0, 0);

    // Format input field
    inputField = newwin(1, INPUT_LEN + 1,
        (LINES - HEIGHT)/2 + 2, (COLS - WIDTH)/2 + HPAD);
}

TextField::~TextField()
{
    delwin(window);
    delwin(inputField);

    // Restore parameters
    noecho();
    curs_set(0);
}

std::string TextField::OpenAndGetInput()
{
    keypad(inputField, true);
    wrefresh(window);

    // Get input string
    char input[INPUT_LEN] = {0, };
    while (input[0] == '\0')
        if (wgetnstr(inputField, input, INPUT_LEN) == KEY_RESIZE)
            WindowManager::RedrawAll();

    return input;
}

void TextField::Redraw()
{
    // Recenter
    mvwin(window, (LINES - HEIGHT)/2,(COLS - WIDTH)/2);
    mvwin(inputField, (LINES - HEIGHT)/2 + 2, (COLS - WIDTH)/2 + HPAD);

    // Restore after cut off by small screen
    if (getmaxx(window) != WIDTH || getmaxy(window) != HEIGHT)
    {
        wresize(window, HEIGHT, WIDTH);
        mvwprintw(window, 1, HPAD,
            std::format("{}\n{}>", text, std::string(HPAD - 1, ' ')).c_str());
        box(window, 0, 0);
    }
    if (getmaxx(inputField) != (INPUT_LEN + 1) || getmaxy(inputField) != 1)
        wresize(inputField, 1, INPUT_LEN + 1);

    // Draw
    wrefresh(window);
}
