#include "game/Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>



int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Game game;
    game.Run();
    
    return 0;
}