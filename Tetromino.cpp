
#include <iostream>
#include "tetromino.h"
#include "board.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <map>
#include <chrono>
#include <random>


// ** this is number of squares from pivot top top rect vert.
const std::map<TETROMINOTYPE, std::vector<sf::Vector2f>> tetVertexMap{
    std::make_pair<TETROMINOTYPE, std::vector<sf::Vector2f>>(TETROMINOTYPE::I, { {-1.5f,-.5f}, {-.5f,-.5f}, {.5f,-.5f}, {1.5f,-.5f} }),
    std::make_pair<TETROMINOTYPE, std::vector<sf::Vector2f>>(TETROMINOTYPE::J, { {-1.f,-1.f}, {-1.f,0.f}, {0.f,0.f}, {1.f,0.f} }),
    std::make_pair<TETROMINOTYPE, std::vector<sf::Vector2f>>(TETROMINOTYPE::L, { {-1.f,0.f}, {0.f,0.f}, {1.f,0.f}, {1.f,-1.f} }),
    std::make_pair<TETROMINOTYPE, std::vector<sf::Vector2f>>(TETROMINOTYPE::O, { {-.5f,-.5f}, {-.5f,.5f}, {.5f,-.5f}, {.5f,.5f} }),
    std::make_pair<TETROMINOTYPE, std::vector<sf::Vector2f>>(TETROMINOTYPE::S, { {-1.f,0.f}, {0.f,0.f}, {0.f,-1.f}, {1.f,-1.f} }),
    std::make_pair<TETROMINOTYPE, std::vector<sf::Vector2f>>(TETROMINOTYPE::T, { {-1.f,0.f}, {0.f,0.f}, {0.f,-1.f}, {1.f,0.f} }),
    std::make_pair<TETROMINOTYPE, std::vector<sf::Vector2f>>(TETROMINOTYPE::Z, { {-1.f,-1.f}, {0.f,-1.f}, {0.f,0.f}, {1.f,0.f} })
};
const std::map<TETROMINOTYPE, sf::Color> tetColorMap{
    std::make_pair(TETROMINOTYPE::I, sf::Color(sf::Color::Cyan)),
    std::make_pair(TETROMINOTYPE::J, sf::Color(sf::Color::Blue)),
    std::make_pair(TETROMINOTYPE::L, sf::Color(255, 165, 0)),
    std::make_pair(TETROMINOTYPE::O, sf::Color(sf::Color::Yellow)),
    std::make_pair(TETROMINOTYPE::S, sf::Color(sf::Color::Green)),
    std::make_pair(TETROMINOTYPE::T, sf::Color(138, 43, 226)),
    std::make_pair(TETROMINOTYPE::Z, sf::Color(sf::Color::Red))
};
const std::map<TETROMINOTYPE, sf::Vector2f> tetStartPos{
    std::make_pair<TETROMINOTYPE, sf::Vector2f>(TETROMINOTYPE::I, {5, 21}),
    std::make_pair<TETROMINOTYPE, sf::Vector2f>(TETROMINOTYPE::J, {4.5, 21.5}),
    std::make_pair<TETROMINOTYPE, sf::Vector2f>(TETROMINOTYPE::L, {4.5, 21.5}),
    std::make_pair<TETROMINOTYPE, sf::Vector2f>(TETROMINOTYPE::O, {5, 22}),
    std::make_pair<TETROMINOTYPE, sf::Vector2f>(TETROMINOTYPE::S, {4.5, 21.5}),
    std::make_pair<TETROMINOTYPE, sf::Vector2f>(TETROMINOTYPE::T, {4.5, 21.5}),
    std::make_pair<TETROMINOTYPE, sf::Vector2f>(TETROMINOTYPE::Z, {4.5, 21.5})
};
namespace {

    void rotate_vert(float angle, sf::Vector2f& vert){
        
        // fast roatation for 90degrees
        if (angle == 90.f) {
            vert = { -vert.y, vert.x };
        }
        else if (angle == 180.f) {
            vert = { -vert.x, -vert.y };
        }
        else if (angle == 270.f) {
            vert = { vert.y, -vert.x };
        }
    }

    
    void boardToWorld(sf::Vector2f topleft, float rectSize,sf::Vector2f& vert) {
        vert.x = vert.x * rectSize + topleft.x + rectSize/2;
        vert.y = (20 - vert.y) * rectSize + topleft.y + rectSize/2;
    }
}


// Generator
TETROMINOTYPE RandomTetrominoGenerator::get(size_t n) {

    while (n >= bag.size()) {
        // add another bag of tetrominos to the stack
        auto seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
        std::vector<TETROMINOTYPE> newbag{ TETROMINOTYPE::I, TETROMINOTYPE::J, TETROMINOTYPE::L, TETROMINOTYPE::O, TETROMINOTYPE::S, TETROMINOTYPE::T, TETROMINOTYPE::Z };
        std::shuffle(newbag.begin(), newbag.end(), std::default_random_engine(seed));
        for (auto& t : newbag) {
            bag.push_back(t);
        }
    }
    return bag[n];
    //return TETROMINOTYPE::I;
}
TETROMINOTYPE RandomTetrominoGenerator::pop() {
    auto t{ get(0) };
    bag.pop_front();
    return t;
}

//////////////////////////
// tetromino Base Class //
//////////////////////////

struct Tetromino::pimpl {
    std::vector<sf::RectangleShape> shape{ 4 };
    
    float rectsize = 0.f;;
    float outlineThickness = 0.f;
    sf::Color outlineColor = sf::Color::Black;
    sf::Color color = sf::Color::Black;

    TETROMINOTYPE type{ TETROMINOTYPE::I };
    sf::Vector2f position{ 0.f,0.f };
    float rotation{ 0.f };
    sf::Rect<float> frame;

};

Tetromino::Tetromino(float rectsize, float outlineThickness, sf::Color outlineColor, sf::FloatRect frame) {
    impl = std::make_unique<pimpl>();
    init(rectsize, outlineThickness, outlineColor, frame);
}
Tetromino::Tetromino() {
    impl = std::make_unique<pimpl>();
};
Tetromino::~Tetromino() = default;

void Tetromino::init(float rectsize, float outlineThickness, sf::Color outlineColor, sf::FloatRect frame) {
    impl->rectsize = rectsize;
    impl->outlineThickness = outlineThickness;
    impl->outlineColor = outlineColor;
    impl->frame = frame;
}

void Tetromino::setType(const TETROMINOTYPE& type) {
    impl->type = type;
    auto color_it{ tetColorMap.find(impl->type) };
    impl->color = color_it->second;


}

void Tetromino::setPosition(const sf::Vector2f& position) {
    impl->position = position;
}
void Tetromino::setRotation(const float& rotation)
{
    impl->rotation = rotation > 360 ? rotation-360.f : rotation;
}
sf::Color Tetromino::getColor() const {
    return impl->color;
}
void Tetromino::update() {
    // get the vertices
    auto vert_it{ tetVertexMap.find(impl->type) };
    auto verts{ vert_it->second };
    
    size_t cntr = 0;
    for (auto& vert : verts) {

        rotate_vert(impl->rotation, vert);
        vert.x = vert.x * impl->rectsize + impl->position.x + impl->frame.left;
        vert.y = vert.y * impl->rectsize + impl->position.y + impl->frame.top;

        auto& s = impl->shape[cntr];

        
        if (!impl->frame.contains(vert) || !impl->frame.contains(vert + sf::Vector2f{ getRectSize()-1, getRectSize()-1 })) {
            s.setOutlineColor(sf::Color::Transparent);
            s.setFillColor(sf::Color::Transparent);
        }
        else
        {
            s.setOutlineColor(impl->outlineColor);
            s.setFillColor(impl->color);
        }

        s.setPosition(vert);
        s.setOutlineThickness(impl->outlineThickness);
        s.setSize({ impl->rectsize,impl->rectsize });

        ++cntr;
    }
}
void Tetromino::draw(sf::RenderWindow& w) {
    for (auto& s : impl->shape) {
        w.draw(s);
    }
}


TETROMINOTYPE Tetromino::getType() const noexcept { return impl->type; }
sf::Vector2f Tetromino::getPosition() const noexcept {return impl->position; }
float Tetromino::getRotation() const noexcept { return impl->rotation; }
float Tetromino::getRectSize() const noexcept { return impl->rectsize; }

///////////////////////////
// activeTetromino Class //
///////////////////////////
struct ActiveTetromino::pimpl {
    Board* Board;
};

ActiveTetromino::ActiveTetromino(float rectsize, float outlineThickness, sf::Color outlineColor, Board* board) {
    impl = std::make_unique<pimpl>();
    sf::FloatRect frame{ board->getTopLeft(), {rectsize * 10.f, rectsize * 20.f} };
    init(rectsize, outlineThickness, outlineColor, frame);
    impl->Board = board;
}


ActiveTetromino::~ActiveTetromino() = default;

// moves are in squares units...
bool ActiveTetromino::canMove(const sf::Vector2f& amount) const {
    auto squares = getBoardSquares();
    for (auto& square : squares) {
        square += amount;
        if (!impl->Board->isEmpty(square)) {
            return false;
        }
    }
    return true;
}
void ActiveTetromino::Move(const sf::Vector2f& amount) 
{
    setPosition(getPosition() + amount * getRectSize());
}

std::vector<sf::Vector2f> ActiveTetromino::getBoardSquares() const {
    auto it{ tetVertexMap.find(getType()) };
    auto verts{ it->second };

    for (auto& vert : verts) {
        rotate_vert(getRotation(), vert);
        vert *= getRectSize();
        vert += getPosition();

        vert.x = (vert.x ) / getRectSize();
        vert.y = 20 - (vert.y ) / getRectSize();
    }

    return verts;
}

// These are all shortcuts for canMove
bool ActiveTetromino::canMoveUp(const float& amount) const { return(canMove({ 0, amount })); }
bool ActiveTetromino::canMoveDown(const float& amount) const { return(canMove({ 0, -amount })); }
bool ActiveTetromino::canMoveLeft(const float& amount) const { return(canMove({ -amount, 0 })); }
bool ActiveTetromino::canMoveRight(const float& amount) const { return(canMove({ amount, 0 })); }
    
// These are all shortcuts for Move 
void ActiveTetromino::MoveUp(const float& amount) { Move({ 0, -amount }); }
void ActiveTetromino::MoveDown(const float &amount){ Move({ 0, amount }); }
void ActiveTetromino::MoveLeft(const float &amount){ Move({ -amount, 0}); }
void ActiveTetromino::MoveRight(const float& amount) { Move({ amount, 0 }); }

void ActiveTetromino::rotate(){
    setRotation( getRotation() + 90.f);

    if (canMove({ 0,0 })) { return; }
    if (canMoveRight(1)) { MoveRight(1); return; }
    if (canMoveLeft(1)) { MoveLeft(1); return; }
    if (canMoveRight(2)) { MoveRight(2); return; }
    if (canMoveLeft(2)) { MoveLeft(2); return; }

    // if we haven't successfully rotated, go back to init rotation
    setRotation(getRotation() - 90.f);
}
void ActiveTetromino::resetPosition() {
    auto it{ tetStartPos.find(getType()) };
    auto pos{ it->second };

    //pos.x = pos.x * getRectSize() + impl->Board->getTopLeft().x + getRectSize() / 2;
    pos.x = pos.x * getRectSize() + getRectSize() / 2;
    //pos.y = (20 - pos.y) * getRectSize() + impl->Board->getTopLeft().y + getRectSize() / 2;
    pos.y = (20 - pos.y) * getRectSize() + getRectSize() / 2;

    setRotation(0.f);

    setPosition(pos);
}
