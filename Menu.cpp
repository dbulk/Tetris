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
    unsigned int titlefontsize{ 150 };
    unsigned int optionfontsize{ 60 };

    sf::Text Title;
    sf::Text Start;
    sf::Text HighScores;
    sf::Text Exit;
    std::vector<sf::Text*> txts{&Title, & Start, &HighScores, &Exit};


    bool in(const int x, const int y, const sf::Text* txt) {
        auto coord{ game->window.mapPixelToCoords({ x,y }) };
        return txt->getGlobalBounds().contains(coord);
    }
    void doMouseOver(sf::Event e) {
        for (auto txt : txts) {
            if (in(e.mouseMove.x, e.mouseMove.y, txt)) {
                if (txt->getFillColor() == sf::Color::White) {
                    txt->setFillColor(sf::Color::Red);
                    game->audio.play(SOUND::DROP);
                }
            }
            else {
                txt->setFillColor(sf::Color::White);
            }
        }
    }

    Game* game;
    bool kms{ false };

    void init() {

        Title = sf::Text("TETRIS", game->font, titlefontsize);
        Start = sf::Text("Start Game", game->font, optionfontsize);
        HighScores = sf::Text("High Scores", game->font, optionfontsize);
        Exit = sf::Text("Exit", game->font, optionfontsize);
        
        float margin = 50;
        float y = height * .1f;
        float x = width / 2.f;
        Title.setOrigin(Title.getLocalBounds().width / 2.f, 0.f);
        Title.setPosition({ x, y });
        

        y = height * .5f;
        Start.setOrigin(Start.getLocalBounds().width / 2.f, 0.f);
        Start.setPosition({ x, y });

        y += Start.getLocalBounds().height + margin;
        HighScores.setOrigin(HighScores.getLocalBounds().width / 2.f, 0.f);
        HighScores.setPosition({ x, y });

        y += HighScores.getLocalBounds().height + margin;
        Exit.setOrigin(Exit.getLocalBounds().width / 2.f, 0.f);
        Exit.setPosition({ x, y });
    }
};

Menu::Menu(Game* game) {
    impl = std::make_unique<pimpl>();
    impl->game = game;
    impl->init();
}

Menu::~Menu() = default;



void Menu::input(const sf::Event& e) {
    if (e.type == sf::Event::MouseMoved) {
        impl->doMouseOver(e);
    }
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Button::Left) {

        if (impl->in(e.mouseButton.x, e.mouseButton.y, &impl->Start)) {
            impl->game->pushState(new Play(impl->game));
            impl->Start.setFillColor(sf::Color::White);
        }
        else if (impl->in(e.mouseButton.x, e.mouseButton.y, &impl->HighScores)) {
            impl->game->pushState(new HighScores(impl->game));
            impl->HighScores.setFillColor(sf::Color::White);
        }
        else if (impl->in(e.mouseButton.x, e.mouseButton.y, &impl->Exit)) {
            impl->kms = true;
        }
    }
}
void Menu::update(float t) {}
void Menu::draw(sf::RenderWindow& w) {
    w.draw(impl->Title);
    w.draw(impl->Start);
    w.draw(impl->HighScores);
    w.draw(impl->Exit);
}
bool Menu::kms() { return impl->kms; }

    
