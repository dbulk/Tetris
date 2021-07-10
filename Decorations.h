#pragma once

#include <memory>
#include <SFML/Graphics/Rect.hpp>

class ScoreKeeper;
enum class TETROMINOTYPE;

namespace sf {
    class RenderWindow;
}


class Decorations {
public:
    Decorations(ScoreKeeper*, sf::FloatRect boardrect, float windowwidth, float rectsize);
    ~Decorations();
    Decorations& operator=(Decorations that);

    void setNextPiece(TETROMINOTYPE);

    void update();
    void draw(sf::RenderWindow& window);

    void setFont(sf::Font f);
private:
    struct pimpl;
    std::unique_ptr<pimpl> impl;
};
