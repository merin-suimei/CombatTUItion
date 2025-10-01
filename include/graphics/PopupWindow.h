#pragma once

#include "graphics/Redrawable.h"
#include <curses.h>
#include <memory>
#include <string>
#include <vector>

class PopupWindow : public Redrawable
{
private:
    PopupWindow(
        const std::vector<std::string> text,
        const std::vector<std::string> buttons);
private:
    WINDOW *window;
    int height;
    int width;
    std::vector<std::string> text;
    std::vector<std::string> buttons;
    std::vector<int> buttonsOffset;

public:
    static std::shared_ptr<PopupWindow> Create(
        const std::vector<std::string> text,
        const std::vector<std::string> buttons);
    ~PopupWindow();

    int OpenAndGetButton();
    void Redraw() override;
};
