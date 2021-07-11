#include "Game.h"
#include "GameState.h"

int main()
{  
    Game game{};
    game.pushState(new Menu(&game));
    //game.pushState(new HighScores(&game));
    game.mainLoop();
    return 0;
}

