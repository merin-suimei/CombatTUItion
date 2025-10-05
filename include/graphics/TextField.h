#pragma once

#include "graphics/Redrawable.h"
#include <curses.h>
#include <string>
#include <memory>

class TextField : public Redrawable
{
private:
    TextField(std::string text);
private:
    WINDOW *window;
    std::string text;
    WINDOW *inputField;

public:
    static std::shared_ptr<TextField> Create(std::string text);
    ~TextField();

    void Redraw() override;
    std::string OpenAndGetInput();
};
