#pragma once
#include <iostream>
#include "SFML/System/Clock.hpp"

class ScoreKeeper {
public:
    void addTet() noexcept {
        tets++;
    }
    void addLines(const size_t numlines) noexcept {
        score += static_cast<unsigned long long>(scoremult[numlines-1]) * static_cast<unsigned long long>(getLevel());
        lines += static_cast<unsigned long>(numlines);
    }

    float getSpeed() noexcept {
        const auto lvl = getLevel();
        if (lvl <= 20) {
            return speeds[lvl-1];
        }
        else {
            return speeds[19] + lvl - 20;
        }
    }

    unsigned long long int getScore() const noexcept { return score; }
    unsigned long int getLines() const noexcept { return lines; }
    unsigned long int getLevel() const noexcept { return lines / 10 + 1; }
    unsigned long int getTotalTets() const noexcept { return tets; }

    float getGameTime() const { return gametime.getElapsedTime().asSeconds(); }


private:
    unsigned long long int score = 0;
    unsigned long int tets = 0;
    unsigned long int lines = 0;
    int scoremult[4]{ 100,300,500,800 };

    // approximating game boy speed:
    float speeds[20]{ 1.1f, 1.2f, 1.3f, 1.5f, 1.6f, 1.8f, 2.1f, 2.7f, 3.5f, 5.5f, 6.f, 6.6f, 7.5f, 8.5f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f };

    sf::Clock gametime;
};