#include "graphics/WindowManager.h"

#include <curses.h>

void WindowManager::RedrawAll()
{
    // Clear screen
    clear();
    refresh();

    // Redraw by order in list and remove expired weak pointers
    for (auto i = redrawableWindows.begin(); i != redrawableWindows.end(); i++)
        if (auto window = i->lock())
            window->Redraw();
        else
            redrawableWindows.erase(i--);
}

void WindowManager::AddWindow(std::weak_ptr<Redrawable> windowPtr)
{
    redrawableWindows.push_back(windowPtr);
}
