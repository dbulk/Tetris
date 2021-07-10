#pragma once

#include <stack>
#include <SFML/Graphics.hpp>
#include "Audio.h"

class GameState;

constexpr float rectsize{ 80.f }, 
                rectOutlineThickness{ -5.f },
                boardTop{ 50 },
                boardLeft{ 50 };

constexpr float width{ static_cast<int>(rectsize * 10 + boardLeft + 700) },
                height{ static_cast<int>(rectsize * 20 + boardTop + 50) },
                ar{ width / height };



struct Game {
    Game();
    ~Game();

    void popState();
    void pushState(GameState* state);
    GameState* currState();

    void mainLoop();

    std::stack<GameState*> statestack;
    sf::RenderWindow window;

    // might move this to an assets class or something like that? I want font in here too?
    Audio audio;
};