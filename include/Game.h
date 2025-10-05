#pragma once

#include "entities/Player.h"

enum GameState
{
    NotStarted, CharacterCreation, Challenge, Stopped
};

class Game
{
private:
    void RunChallenge();
    void CreateCharacter();
private:
    GameState gameState = NotStarted;
    Player *player = nullptr;

public:
    Game();
    ~Game();

    void Start();
    GameState getGameState() const;
};
