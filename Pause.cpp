#include "GameState.h"
#include "Game.h"
#include "Decorations.h"
#include "Board.h"
#include "Tetromino.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics.hpp>

struct Pause::pimpl {
    Game* game{ nullptr };
    Board* board{ nullptr };
    Decorations* decorations{ nullptr };
    Tetromino* tetromino{ nullptr };
    bool kms = false;

    sf::RectangleShape overlay;
    sf::Text pausetext;
};


Pause::Pause(Game* game, Board* board, Decorations* decorations, Tetromino* tetromino) {
    impl = std::make_unique<pimpl>();
    impl->game = game;
    impl->board = board;
    impl->decorations = decorations;
    impl->tetromino = tetromino;

    impl->overlay = sf::RectangleShape({ width,height });
    impl->overlay.setFillColor(sf::Color(0, 0, 0, 210));

    impl->pausetext = sf::Text("Pause", impl->game->font);
    impl->pausetext.setCharacterSize(100);
    const auto s{ impl->pausetext.getLocalBounds() };

    impl->pausetext.setOrigin({ s.width / 2,s.height / 2 });
    impl->pausetext.setPosition({ width / 2,height / 2 });

}
Pause::~Pause() = default;

void Pause::input(const sf::Event& e) noexcept {
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) {
        impl->kms = true;
    }

}
void Pause::update(float t) noexcept  {

    
}
void Pause::draw(sf::RenderWindow& window) {
    impl->decorations->draw(window);
    impl->board->draw(window);
    impl->tetromino->draw(window);
    
    window.draw(impl->overlay);
    window.draw(impl->pausetext);
}


bool Pause::kms() const noexcept {
    if (impl->kms) {
        impl->game->audio.play();
    }
    return impl->kms;
}
    