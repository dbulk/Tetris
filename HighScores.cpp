#include "HighScores.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "ScoreKeeper.h"
#include "GameState.h"
#include "Game.h"
#include <SFML/Window/Event.hpp>
namespace {
    std::string maketimestring(int t) {
        int days = t / (3600 * 24); t -= days * 3600 * 24;
        int hours = t / 3600; t -= hours * 3600;
        int minutes = t / 60; t -= minutes * 60;
        int seconds = t;

        std::string out;
        if (days) { out += std::to_string(days) + " d "; }
        if (hours) { out += std::to_string(hours) + " h "; }
        if (minutes) { out += std::to_string(minutes) + " m "; }
        out += std::to_string(seconds) + " s";

        return out;
    }
}
struct HighScores::pimpl {
    
    void readFile();
    void writeFile();

    unsigned long long int bestscore = 0;
    unsigned long int bestlevel = 1;
    unsigned long int totaltets = 0;
    unsigned long int totallines = 0;

    int totalgames = 0;
    int totaltime = 0;
    
    std::fstream file;

    bool kms{ false };

    void init(const sf::Font &font) {

        float x = width / 2;
        float y = height * .1;
        
        txtscore.setString("Best Score: " + std::to_string(bestscore));
        txtlevel.setString("Best Level: " + std::to_string(bestlevel));
        txttets.setString("Pieces Dropped: " + std::to_string(totaltets));
        txtlines.setString("Lines Cleared: " + std::to_string(totallines));
        txtgames.setString("Games Played: " + std::to_string(totalgames));
        txttime.setString("Time Played: " + maketimestring(totaltime));
        txtBackToMenu.setString("Return to Menu");


        for (auto t : txts) {
            t->setFont(font);
            t->setCharacterSize(50);
            t->setPosition({ x,y });
            t->setOrigin({ t->getLocalBounds().width / 2,t->getLocalBounds().height / 2 });
            y += 200;
        }
        txtBackToMenu.setPosition({ x,y });
    }

    sf::Text txtscore;
    sf::Text txtlevel;

    sf::Text txttets;
    sf::Text txtlines;
    sf::Text txttime;
    sf::Text txtgames;
    sf::Text txtBackToMenu;
    std::vector<sf::Text*> txts{ &txtscore, &txtlevel, &txttets, &txtlines, &txttime, &txtgames, &txtBackToMenu };

    Game *game;
};


HighScores::HighScores(Game* game) {
    impl = std::make_unique<pimpl>();
    impl->readFile();
    impl->game = game;
    if (!game->font.getInfo().family.empty() ) {
        impl->init(game->font);
    }    
}
HighScores::~HighScores() = default;


void HighScores::addGameToHighScores(ScoreKeeper* sc) {
    impl->readFile();
    impl->bestscore = std::max(sc->getScore(), impl->bestscore);
    impl->bestlevel = std::max(sc->getLevel(), impl->bestlevel);
    impl->totaltets += sc->getTotalTets();
    impl->totallines += sc->getLines();
    impl->totaltime += static_cast<int>(sc->getGameTime());
    impl->totalgames++;
    impl->writeFile();
}
unsigned long long int HighScores::getBestScore() const noexcept { return impl->bestscore; };
unsigned long long int HighScores::getBestLevel() const noexcept { return impl->bestlevel; };
void HighScores::input(const sf::Event& event) {
    auto& txt = impl->txtBackToMenu;

    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
        impl->kms = true;
    }
    else if (event.type == sf::Event::MouseMoved) {
        auto coord{ impl->game->window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y}) };
        if (txt.getGlobalBounds().contains(coord)) {
            if (txt.getFillColor() == sf::Color::White) {
                txt.setFillColor(sf::Color::Red);
                impl->game->audio.play(SOUND::DROP);
            }
        }
        else {
            txt.setFillColor(sf::Color::White);
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Left) {
        auto coord{ impl->game->window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y}) };
        if (txt.getGlobalBounds().contains(coord)) {
            impl->kms = true;
        }
    }
}
void HighScores::update(float t) {}
void HighScores::draw(sf::RenderWindow& window) {
    for (auto txt : impl->txts) {
        window.draw(*txt);
    }
}
bool HighScores::kms() {
    return impl->kms;
}
void HighScores::pimpl::readFile() {
    file.open("scores.dat", std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open()) {
        if (file.tellg() > 0) {
            file.seekg(0, std::ios::beg);
            char version{ 0 };
            file.read(&version, sizeof(version));
            if (version == 1) {
                file.read((char*)&bestscore, sizeof(bestscore));
                file.read((char*)&bestlevel, sizeof(bestlevel));
                file.read((char*)&totaltets, sizeof(totaltets));
                file.read((char*)&totallines, sizeof(totallines));
                file.read((char*)&totaltime, sizeof(totaltime));
                file.read((char*)&totalgames, sizeof(totalgames));
            }
            file.close();
        }
    }
    else {
        writeFile();
    }
}
void HighScores::pimpl::writeFile() {
    file.open("scores.dat", std::ios::out | std::ios::binary);
    if (file.is_open()) {
        file.clear();
        const char version{ 1 };

        file.write(&version, sizeof(version));
        file.write((char*)&bestscore, sizeof(bestscore));
        file.write((char*)&bestlevel, sizeof(bestlevel));
        file.write((char*)&totaltets, sizeof(totaltets));
        file.write((char*)&totallines, sizeof(totallines));
        file.write((char*)&totaltime, sizeof(totaltime));
        file.write((char*)&totalgames, sizeof(totalgames));
        file.close();
    }

    std::cout << "Score:" << bestscore << std::endl;
    std::cout << "lvl:" << bestlevel << std::endl;
    std::cout << "tets:" << totaltets << std::endl;
    std::cout << "lines:" << totallines << std::endl;
    std::cout << "time:" << totaltime << std::endl;

}