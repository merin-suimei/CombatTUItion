#include "graphics/WindowManager.h"

#include <curses.h>

void WindowManager::RedrawAll()
{
    #ifdef _WIN32
    resize_term(0, 0);
    #endif
    // Clear screen
    clear();
    refresh();

    // Redraw by order in list and remove expired weak pointers
    for (auto i = redrawableWindows.begin(); i != redrawableWindows.end(); /* */ )
        if (auto window = i->lock())
        {
            window->Redraw();
            ++i;
        }
        else
            i = redrawableWindows.erase(i);
}

void WindowManager::AddWindow(std::weak_ptr<Redrawable> windowPtr)
{
    redrawableWindows.push_back(windowPtr);
}
