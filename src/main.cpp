#include "include/no_gravity_game_ws/game.hpp"

int main()
{
    Game game;

    while (!game.getWindow()->isDone())
    {
        game.handleInput();
        game.update();
        game.render();
    }

    return 0;
}