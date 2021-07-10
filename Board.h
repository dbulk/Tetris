#pragma once
#include <SFML/System/Vector2.hpp> 
#include <vector>
#include <memory>

namespace sf {
    class Color;
    class RenderWindow;
}

class Board
{
public:
    Board(float rectsize, float outlinethickness, sf::Vector2f topLeft);
    ~Board();

    void setSquaresToColor(std::vector<sf::Vector2f> Squares, sf::Color clr);
    
    bool isEmpty(int x, int y) const;
    bool isEmpty(sf::Vector2f) const;
    size_t nLinesToClear() const;

    void update();
    void animateold(float proportion, sf::RenderWindow& window);
    void animate(float proportion);
    void draw(sf::RenderWindow& window);
    
    void resetOutlineThickness();
    void clearEmptyLines();

    sf::Vector2f getTopLeft() const;

private:
    struct pimpl;
    std::unique_ptr<pimpl> impl;
};

