#pragma once

#include <stack>
#include <SFML/Graphics.hpp>
#include "Audio.h"
#include "HighScores.h"

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

    void popState() noexcept;
    void pushState(GameState* state);
    GameState* currState() const noexcept;

    void mainLoop();

    std::stack<GameState*> statestack;
    sf::RenderWindow window;

    // might move there to an asset class?
    Audio audio{ width,height };
    sf::Font font;

    HighScores highscores=HighScores(this);
};