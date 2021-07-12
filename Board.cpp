
#include <iostream>
#include "board.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

struct Board::pimpl {
    const size_t width{ 10 };
    const size_t height{ 22 };
    std::vector<std::vector<sf::RectangleShape>> shapemap;
    sf::RectangleShape border;

    float rectsize{ 0.f };
    float outlinethickness{ 0.f };
    sf::Vector2f topleft;

    std::vector<size_t> linestoclear;

    void initboard() {
        shapemap.resize(width);
        for (size_t col = 0; col < width; ++col) {
            shapemap[col].resize(height);
            for (size_t row = 0; row < height; ++row) {
                auto& s = shapemap[col][row];

                s.setSize({ rectsize,rectsize });
                s.setFillColor(sf::Color::Black);
                const sf::Vector2f pos{
                    col * rectsize + topleft.x,
                    (20-row) * rectsize + topleft.y
                };
                s.setPosition(pos);

                s.setOutlineColor(sf::Color::Black);
                s.setOutlineThickness(outlinethickness);
            } 
        }
    }
    void checkLines(std::vector<size_t> ys) {
        std::sort(ys.begin(), ys.end());
        auto last = std::unique(ys.begin(), ys.end());
        ys.erase(last, ys.end());

        for (auto& y : ys) {
            if (isFullLine(y)) {
                linestoclear.push_back(y);
            }
        }
    }
    bool isFullLine(size_t y) {
        for (size_t x = 0; x < width; ++x) {
            if (!hasColorUnsafe(x, y)) { return false; };
        }
        return true;
    }
    bool hasColorUnsafe(size_t x, size_t y) {
        // Doesn't check against board size!
        return shapemap[x][y].getFillColor() != sf::Color::Black;
    }
    void clearEmptyLines() {
        if (!linestoclear.empty()) {
            std::sort(linestoclear.begin(), linestoclear.end());
            std::reverse(linestoclear.begin(), linestoclear.end());

            for (auto y : linestoclear) {
                clearEmptyLine(y);
            }
            linestoclear.clear();
        }
    }
    void clearEmptyLine(size_t y){
        for (; y < height-1; ++y) {
            for (size_t x = 0; x < width; ++x) {
                shapemap[x][y].setFillColor(shapemap[x][y + 1].getFillColor());
            }
        }
    }
};

Board::Board(float rectsize, float outlinethickness, sf::Vector2f topleft) {
    impl = std::make_unique<pimpl>();

    impl->rectsize = rectsize;
    impl->topleft = topleft;
    impl->outlinethickness = outlinethickness;
    impl->border.setPosition(topleft);
    impl->border.setSize({ rectsize * 10,rectsize * 20 });
    impl->border.setFillColor(sf::Color::Black);
    impl->border.setOutlineColor(sf::Color(100,100,100));
    impl->border.setOutlineThickness(5);
    

    impl->initboard();
}
Board::~Board() = default;


void Board::setSquaresToColor(std::vector<sf::Vector2f> Squares, sf::Color clr) {

    std::vector<size_t> yvalues;
    for (auto& square : Squares) {
        auto x{ static_cast<size_t>(square.x) };
        auto y{ static_cast<size_t>(square.y) };
        x = std::min(x, impl->width - 1);
        y = std::min(y, impl->height - 1);

        impl->shapemap[x][y].setFillColor(clr);
        yvalues.push_back(y);
    }
    impl->checkLines(yvalues);
}    
bool Board::isEmpty(sf::Vector2f v) const {
    return(isEmpty(static_cast<int>(v.x), static_cast<int>(v.y)));
}
bool Board::isEmpty(int x, int y) const {
    if (x < 0 || x >= impl->width || y <= 0) {
        return false;
    }
    if (y >= impl->height) {
        return true;
    }
    return !impl->hasColorUnsafe(static_cast<size_t>(x), static_cast<size_t>(y));
}

size_t Board::nLinesToClear() const noexcept {
    return impl->linestoclear.size();
}
sf::Vector2f Board::getTopLeft() const noexcept {
    return impl->topleft;
}

void Board::clearEmptyLines() {
    impl->clearEmptyLines();
}

void Board::update() noexcept {
    //impl->clearEmptyLines();
}

void Board::animateold(float proportion, sf::RenderWindow& window) {
    for (auto y : impl->linestoclear) {
        for (size_t x = 0; x < impl->width; ++x) {
            impl->shapemap[x][y].setOutlineThickness(impl->outlinethickness + powf(proportion, 2.f) * -impl->rectsize / 2);

        }
    }
    draw(window);
        

    for (auto y : impl->linestoclear) {
        for (size_t x = 0; x < impl->width; ++x) {
            impl->shapemap[x][y].setOutlineThickness(impl->outlinethickness);
        }
    }
}

void Board::animate(float proportion) {
    for (size_t x = 0; x < impl->width; ++x) {
        for (auto y : impl->linestoclear) {
            impl->shapemap[x][y].setOutlineThickness(impl->outlinethickness + powf(proportion, 2.f) * -impl->rectsize / 2);
            
        }
    }
}

void Board::resetOutlineThickness() {
    for (auto& col : impl->shapemap) {
        for (auto& shape : col) {
            shape.setOutlineThickness(impl->outlinethickness);
        }
    }
}

void Board::draw(sf::RenderWindow& window) {
    window.draw(impl->border);
    for (auto& col : impl->shapemap) {
        for (auto& shape : col) {
            if (shape.getFillColor() != sf::Color::Black) {
                window.draw(shape);
            }
        }
    }
}

