#pragma once

#include "graphics/Redrawable.h"
#include <list>
#include <memory>

class WindowManager
{
private:
    inline static std::list<std::weak_ptr<Redrawable>> redrawableWindows;

public:
    static void RedrawAll();
    static void AddWindow(std::weak_ptr<Redrawable> windowPtr);
};
