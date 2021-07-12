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
    HighScores(const HighScores&) {};

    void addGameToHighScores(const ScoreKeeper* const sc);
    unsigned long long int getBestScore() const noexcept;
    unsigned long long int getBestLevel() const noexcept;
    
    void input(const sf::Event& event) override;
    void update(float t) noexcept override;
    void draw(sf::RenderWindow&) override;

    bool kms() const noexcept override;

private:
    struct pimpl;
    //std::unique_ptr<pimpl> impl;
    pimpl* impl;
};