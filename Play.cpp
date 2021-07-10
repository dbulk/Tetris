#include <iostream>

#include <SFML/Graphics.hpp>

//#include "Audio.h"
#include "Board.h"
#include "Decorations.h"
#include "Game.h"
#include "Movement.h"
#include "ScoreKeeper.h"
#include "Tetromino.h"
#include "GameState.h"



struct Play::pimpl {
    Game* game;
    
    Board board{ rectsize, rectOutlineThickness, { boardLeft, boardTop} };
    ActiveTetromino tetromino{ rectsize, rectOutlineThickness, sf::Color::Black, &board };
    Decorations decorations{ &score, sf::FloatRect{boardLeft,boardTop,10 * rectsize,20 * rectsize}, width, rectsize };;
    Movement gravity;
    Movement leftmove{ 3.f, 30.f };
    Movement rightmove{ 3.f, 30.f };
    RandomTetrominoGenerator tbag;
    ScoreKeeper score;

    bool doDrop{ false };
    bool doAnim{ false };
    bool doGameOver{ false };
    float animclock = 0.f;

    bool kms;

    void input(const sf::Event& e);
    void animate(float t);
    void gameover();
    void update(float t);
    bool handleMovement();
    void commitTet();
};


Play::Play(Game* game) {
    impl = std::make_unique<pimpl>();

    impl->gravity.setActive(true);
    impl->tetromino.setType(impl->tbag.pop());
    impl->tetromino.resetPosition();
    impl->decorations.setNextPiece(impl->tbag.get());
    impl->decorations.setFont(game->font);

    impl->game = game;
    impl->game->audio.playloop("thememusic.wav", 0, 77);
}
Play::~Play() = default;




void Play::input(const sf::Event& e) {
    impl->input(e);
}

void Play::update(float t){ 
    if (impl->doAnim) {
        impl->animate(t);
    }
    else if (impl->doGameOver) {
        impl->gameover();
    }
    else {
        impl->update(t);
        impl->board.update();
        impl->tetromino.update();
        impl->decorations.update();
    }

}
void Play::draw(sf::RenderWindow& w) {
    impl->board.draw(w);
    impl->tetromino.draw(w);
    impl->decorations.draw(w);

}

void Play::pimpl::input(const sf::Event& e) {
    switch (e.type)
    {
    case sf::Event::KeyPressed:
        switch (e.key.code)
        {
        case sf::Keyboard::Key::Up:
            tetromino.rotate();
            break;

        case sf::Keyboard::Key::Left:

            if (tetromino.canMoveLeft()) {
                tetromino.MoveLeft();
            }
            leftmove.resetTime();
            leftmove.setActive(true);
            leftmove.setFast(false);
            break;

        case sf::Keyboard::Key::Right:
            if (tetromino.canMoveRight()) {
                tetromino.MoveRight();
            }
            rightmove.resetTime();
            rightmove.setActive(true);
            rightmove.setFast(false);
            break;

        case sf::Keyboard::Key::Space:
            doDrop = true;
            break;
        case sf::Keyboard::Key::Escape:
            game->audio.pause();
            game->pushState(new Pause(game, &board, &decorations, &tetromino));
        }
    
        break;
    case sf::Event::KeyReleased:
        // can these fail because of timing??
        switch (e.key.code) {
        case sf::Keyboard::Key::Left:
            leftmove.setActive(false);
            break;
        case sf::Keyboard::Key::Right:
            rightmove.setActive(false);
            break;
        }
    }
}

void Play::pimpl::update(float t) {
    bool didAdd = handleMovement();
    if (didAdd) {
        score.addTet();
        decorations.setNextPiece(tbag.get());

        if (board.nLinesToClear() > 0) {
            score.addLines(board.nLinesToClear());
            board.nLinesToClear() == 4 ? game->audio.play(SOUND::TETRIS) : game->audio.play(SOUND::CLEAR);
            animclock = 0.f;
            doAnim = true;
        }
        else {
            game->audio.play(SOUND::DROP);
        }
    }
    gravity.addTime(t);
    leftmove.addTime(t);
    rightmove.addTime(t);
}
void Play::pimpl::commitTet() {
    board.setSquaresToColor(tetromino.getBoardSquares(), tetromino.getColor());
    tetromino.setType(tbag.pop());
    gravity.resetTime();
    doDrop = false;

    tetromino.resetPosition();
    if (!tetromino.canMove({ 0,0 })) { doGameOver = true; };
}
bool Play::pimpl::handleMovement() {
    if (doDrop) {
        size_t i = 0;

        while (tetromino.canMoveDown(float(++i))) { }
        tetromino.MoveDown(float(i-1));

        commitTet();
        return true;
    }

    gravity.setFast(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down));

    while (gravity.hasMovement()) {
        gravity--;
        if (tetromino.canMoveDown()) { tetromino.MoveDown(); }
        else
        {
            commitTet();
            return true;
        }
    }
    
    while (leftmove.hasMovement()) {
        leftmove--;
        if (tetromino.canMoveLeft()) {
            tetromino.MoveLeft();
            leftmove.setFast(true);
        }
    }
        
    while (rightmove.hasMovement()) {
        rightmove--;
        if (tetromino.canMoveRight()) {
            tetromino.MoveRight();
            rightmove.setFast(true);
        }
    }
    return false;
}
void Play::pimpl::gameover() {
    game->audio.stop();

    game->audio.play(SOUND::GAMEOVER);
    while (game->audio.isPlaying()) {}
    // todo: I think I probably want to end up with a death screen rather than going back to the main menu immediately
    // todo: I can't pop myself...shit how do i pop myself?
    //game->popState();
    kms = true;
}
void Play::pimpl::animate(float t) {
    //
    board.animate(animclock); // this takes proportion...so if animation times other than 1 second adjust here
    animclock += t;

    if (animclock > 1) {
        board.resetOutlineThickness();
        board.clearEmptyLines();
        doAnim = false;
    }
}

bool Play::kms() {
    return impl->kms;
}