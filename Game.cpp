#include "Game.h"
#include "GameState.h"
#include <iostream>

Game::Game() { 
    font.loadFromFile("neuropol.ttf");

    window.create(sf::VideoMode{ static_cast<int>(width), static_cast<int>(height) }, "tetris");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    
}
Game::~Game() {
    while (!statestack.empty()) {
        popState();
    }
}
void Game::popState() {
    delete statestack.top(); // todo: just make it a uniqueptr?
    statestack.pop();
}
void Game::pushState(GameState* state) {
    statestack.push(state);
}
GameState* Game::currState() {
    if (statestack.empty()) {
        return nullptr;
    }
    return statestack.top();
}

void Game::mainLoop() {
    sf::Clock clock;
    sf::Event event;
    sf::View view(sf::FloatRect(0.f, 0.f, width, height));
    window.setView(view);
    while(currState())
    {
        float t = clock.restart().asSeconds();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            else if (event.type == sf::Event::Resized) {
                sf::FloatRect va;
                float actual_ar{float(event.size.width) / float(event.size.height) };
                float k = actual_ar / ar;

                va = actual_ar > ar ? 
                        sf::FloatRect((width - width*k)/2 , 0, width*k, height ) :
                        sf::FloatRect(0, (height-height / k) / 2, width, height / k);
                window.setView(sf::View(va));
                
            }
            currState()->input(event);
        }
        
        if (window.hasFocus()) {
            currState()->update(t);
            window.clear(sf::Color(10,10,10));
            currState()->draw(window);
            window.display();

            if (currState()->kms()) {
                popState();
            }
        }
    }
    window.close();

}