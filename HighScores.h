#pragma once

#include "GameState.h"
class ScoreKeeper;
struct Game;
namespace sf {
    class Event;
}

class HighScores : public GameState
{
public:
    HighScores(Game*);
    ~HighScores();

    void addGameToHighScores(ScoreKeeper* sc);
    unsigned long long int getBestScore() const noexcept;
    unsigned long long int getBestLevel() const noexcept;
    
    void input(const sf::Event& event);
    void update(float t);
    void draw(sf::RenderWindow&);

    bool kms();

private:
    struct pimpl;
    std::unique_ptr<pimpl> impl;
};