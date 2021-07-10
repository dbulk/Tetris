#pragma once

#include <vector>
#include <deque>
#include <memory>
#include <SFML/System/Vector2.hpp> 
#include <SFML/Graphics/Rect.hpp>
//#include <SFML/Graphics/Color.hpp>

namespace sf {
    class Color;
    class RenderWindow;
}

class Board;

enum class TETROMINOTYPE {I,J,L,O,S,T,Z};

class RandomTetrominoGenerator {
public:
    TETROMINOTYPE get(size_t n = 0);
    TETROMINOTYPE pop();
private:
    std::deque<TETROMINOTYPE> bag;
};

class Tetromino {
public:
    Tetromino(float rectsize, float outlineThickness, sf::Color outlineColor, sf::Rect<float>frame);
    Tetromino();
    ~Tetromino();
    void setType(const TETROMINOTYPE&);
    void setPosition(const sf::Vector2f&);
    void setRotation(const float&);
    void update();
    void draw(sf::RenderWindow&);

    TETROMINOTYPE getType() const noexcept;
    sf::Vector2f  getPosition() const noexcept;
    float getRotation() const noexcept;
    float getRectSize() const noexcept;
    sf::Color getColor() const;

    
protected:
    void init(float rectsize, float outlineThickness, sf::Color outlineColor, sf::FloatRect frame);

private:
    struct pimpl;
    std::unique_ptr<pimpl> impl;//{ std::make_unique<pimpl>() };
};


class ActiveTetromino : public Tetromino {
public:
    ActiveTetromino(float rectsize, float outlineThickness, sf::Color outlineColor, Board* board);
    ~ActiveTetromino();
    bool canMove(const sf::Vector2f&) const;
    void Move( const sf::Vector2f&);

    // These are all shortcuts for canMove
    bool canMoveUp(const float &amount = 1) const;
    bool canMoveDown(const float &amount = 1) const;
    bool canMoveLeft(const float &amount = 1) const;
    bool canMoveRight(const float &amount = 1) const;
    
    // These are all shortcuts for Move 
    void MoveUp(const float& amount = 1);
    void MoveDown(const float &amount = 1);
    void MoveLeft(const float &amount = 1);
    void MoveRight(const float &amount = 1);
    void rotate();
    std::vector<sf::Vector2f> getBoardSquares() const;
    void resetPosition(); // reset to the top
    
private:
    struct pimpl;
    std::unique_ptr<pimpl> impl;//{ std::make_unique<pimpl>() };
};

// TODO: 
//  1. shadow version
//  2. next thingy version