#pragma once

#include <memory>

struct Game;
class Decorations;
class Board;
class Tetromino;

namespace sf {
    class RenderWindow;
    class Event;
}

class GameState
{
public:
    virtual void input(const sf::Event&) = 0;
    virtual void update(float t) = 0;
    virtual void draw(sf::RenderWindow&) = 0; //todo: would be cool if draw was const...
    virtual bool kms() = 0; // a state can't pop itself, so it send a message?
};

class Play : public GameState
{
public:
    Play(Game*);
    ~Play();
    void input(const sf::Event&) override;
    void update(float t) override;
    void draw(sf::RenderWindow&) override;
    bool kms() override;

private:
    struct pimpl;
    std::unique_ptr<pimpl> impl;
};

class Menu : public GameState
{
public:
    Menu(Game*);
    ~Menu();
    void input(const sf::Event&) override;
    void update(float t) override;
    void draw(sf::RenderWindow&) override;
    bool kms() override;
    
private:
    struct pimpl;
    std::unique_ptr<pimpl> impl;
};

class Pause : public GameState
{
public:
    Pause(Game*, Board*, Decorations*, Tetromino*);
    ~Pause();
    void input(const sf::Event&) override;
    void update(float t) override;
    void draw(sf::RenderWindow&) override;
    bool kms() override;
    
private:
    struct pimpl;
    std::unique_ptr<pimpl> impl;
};
