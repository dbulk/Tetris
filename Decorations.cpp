#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include "decorations.h"
#include "scorekeeper.h"
#include "tetromino.h"


// What are we doing here?
//  We have some static texts to place
//  Some layout to do
//  Some stuff to update...
//  some magic fontsize garbage to make sure we don't overflow...


float margin = 50;

struct Decorations::pimpl {
    sf::Font font;
    unsigned int fontsize{ 60 };
    
    sf::Text lblLevel{ "Level", font };
    sf::Text lblScore{ "Score", font };
    sf::Text lblNext{ "Next", font };
    sf::Text lblLines{ "Lines", font };
    sf::Text lblPieces{ "Pieces", font };

    sf::Text txtLevel{ "", font };
    sf::Text txtScore{ "", font };
    sf::Text txtLines{ "", font };
    sf::Text txtPieces{ "", font };
    
    void inittext() {
        for (auto t : allTexts) {
            t->setCharacterSize(fontsize);
        }
        auto y = rightrect.top;
        lblNext.setPosition(rightrect.left, y);

        y += fontsize + margin + rectsize *3;
        lblLevel.setPosition(rightrect.left, y);
        txtLevel.setPosition(rightrect.left+rightrect.width-margin, y);

        y += fontsize + margin;
        lblLines.setPosition(rightrect.left, y);
        txtLines.setPosition(rightrect.left+rightrect.width-margin, y);
        
        y += fontsize + margin;
        lblPieces.setPosition(rightrect.left, y);
        txtPieces.setPosition(rightrect.left+rightrect.width-margin, y);

        y += fontsize + margin*2;
        lblScore.setPosition(rightrect.left, y);
        txtScore.setPosition(rightrect.left+rightrect.width-margin, y + fontsize*1.2);
    }

    std::unique_ptr<Tetromino> tet;
    ScoreKeeper* score;
    sf::FloatRect rightrect;
    float rectsize;

    std::vector<sf::Text*> allTexts{ &lblLevel,&lblScore,&lblNext,&lblLines,&lblPieces,&txtLevel,&txtScore,&txtLines,&txtPieces };
};

Decorations::Decorations(ScoreKeeper* score, sf::FloatRect boardrect, float windowwidth, float rectsize) {
    impl = std::make_unique<pimpl>();

    impl->score = score;
    impl->rightrect = sf::FloatRect(boardrect.left + boardrect.width + margin, boardrect.top, windowwidth-boardrect.width-margin*2, boardrect.height);
    impl->rectsize = rectsize;

    impl->inittext();
    
    sf::FloatRect nextrect{ impl->rightrect.left, impl->lblNext.getGlobalBounds().top + impl->lblNext.getGlobalBounds().height, impl->rightrect.width, rectsize * 3 };
    impl->tet = std::make_unique<Tetromino>(rectsize, -5.f, sf::Color::Black, nextrect);
    impl->tet->setPosition({ nextrect.width/2-rectsize/2, nextrect.height/2});
}
Decorations::~Decorations() = default;


void Decorations::setNextPiece(TETROMINOTYPE type) { impl->tet->setType(type); }
void Decorations::update() {
    impl->txtLevel.setString(std::to_string(impl->score->getLevel()));
    impl->txtLevel.setOrigin({ impl->txtLevel.getLocalBounds().width, 0 });
    
    impl->txtLines.setString(std::to_string(impl->score->getLines()));
    impl->txtLines.setOrigin({ impl->txtLines.getLocalBounds().width, 0 });
    
    impl->txtPieces.setString(std::to_string(impl->score->getTotalTets()));
    impl->txtPieces.setOrigin({ impl->txtPieces.getLocalBounds().width, 0 });
    
    impl->txtScore.setString(std::to_string(impl->score->getScore()));
    impl->txtScore.setOrigin({ impl->txtScore.getLocalBounds().width, 0 });

    impl->tet->update();
}
void Decorations::draw(sf::RenderWindow& window) {
    for (auto t : impl->allTexts) {
        window.draw(*t);
    }
    impl->tet->draw(window);
    
}


void Decorations::setFont(sf::Font font) {
    impl->font = font;
}