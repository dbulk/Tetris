#include <iostream>

#include "GameState.h"

#include "Game.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>


/*
* Title
* Options:
*   Start Game
*   Starting Level?
*   Stats/High Scores?
*   Exit
*   Controls?
*/

struct Menu::pimpl {
    sf::Font font;
    unsigned int titlefontsize;
    unsigned int optionfontsize;

    sf::Text Title{ "TETRIS", font };
    sf::Text Start{ "Start Game", font };
    sf::Text Exit{ "Exit", font };

    bool inStart(const int x, const int y) {
        auto coord{ game->window.mapPixelToCoords({ x,y }) };
        return Start.getGlobalBounds().contains(coord);
    }
    bool inExit(const int x, const int y) {
        auto coord{ game->window.mapPixelToCoords({ x,y }) };
        return Exit.getGlobalBounds().contains(coord);
    }

    Game* game;
    bool kms{ false };
};

Menu::Menu(Game* game) {
    impl = std::make_unique<pimpl>();
    impl->game = game;

    impl->font.loadFromFile("neuropol.ttf");
    impl->titlefontsize = static_cast<int>(150);

    float margin = 50;
    float y = height * .1f;
    float x = width / 2.f;
    impl->Title.setCharacterSize(impl->titlefontsize);
    impl->Title.setPosition({ x, y });
    impl->Title.setOrigin(impl->Title.getLocalBounds().width / 2.f, 0.f);

     y = height * .5f;

    impl->optionfontsize = static_cast<int>(60);
    impl->Start.setCharacterSize(impl->optionfontsize);
    impl->Start.setOrigin(impl->Start.getLocalBounds().width / 2.f, 0.f);
    impl->Start.setPosition({ x, y });
    y += impl->Start.getLocalBounds().height + margin;
    
    impl->Exit.setCharacterSize(impl->optionfontsize);
    impl->Exit.setOrigin(impl->Exit.getLocalBounds().width / 2.f, 0.f);
    impl->Exit.setPosition({ x, y });
    
}

Menu::~Menu() = default;

void Menu::input(const sf::Event& e) {
    if (e.type == sf::Event::MouseMoved) {
        if (impl->inStart(e.mouseMove.x, e.mouseMove.y)){
            if (impl->Start.getFillColor() == sf::Color::White) {
                impl->Start.setFillColor(sf::Color::Red);
                impl->game->audio.play(SOUND::DROP);
            }
        }
        else
        {
            impl->Start.setFillColor(sf::Color::White);
        }
        if (impl->inExit(e.mouseMove.x, e.mouseMove.y)){
            if (impl->Exit.getFillColor() == sf::Color::White) {
                impl->Exit.setFillColor(sf::Color::Red);
                impl->game->audio.play(SOUND::DROP);
            }
        }
        else
        {
            impl->Exit.setFillColor(sf::Color::White);
        }
    }
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Button::Left) {
        if (impl->inStart(e.mouseButton.x, e.mouseButton.y)) {
            impl->game->pushState(new Play(impl->game));
        }
        else if (impl->inExit(e.mouseButton.x, e.mouseButton.y)) {
            impl->kms = true;
        }
    }
}
void Menu::update(float t) {}
void Menu::draw(sf::RenderWindow& w) {
    w.draw(impl->Title);
    w.draw(impl->Start);
    w.draw(impl->Exit);
}
bool Menu::kms() { return impl->kms; }

    
