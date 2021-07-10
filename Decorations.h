#pragma once

#include <memory>
class ScoreKeeper;
enum class TETROMINOTYPE;
namespace sf {
    class RenderWindow;
}


class Decorations {
public:
    Decorations(ScoreKeeper*, sf::FloatRect boardrect, float windowwidth, float rectsize);
    ~Decorations();

    void setNextPiece(TETROMINOTYPE);

    void update();
    void draw(sf::RenderWindow& window);

private:
    struct pimpl;
    std::unique_ptr<pimpl> impl;
};
