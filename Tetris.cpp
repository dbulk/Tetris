#include "Game.h"
#include "GameState.h"

int main()
{  
    Game game{};
    
    //game.pushState(new Play(&game));
    game.pushState(new Menu(&game));
    game.mainLoop();

    return 0;
}

