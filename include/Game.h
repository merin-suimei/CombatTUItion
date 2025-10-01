#pragma once

#include "entities/Player.h"

class Game
{
private:
    bool running = false;
    Player player = Player("Nameless"); // Temp

public:
    Game();
    ~Game();

    void Start();
    bool IsRunning() const;
};
