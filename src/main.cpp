#include "include/no_gravity_game_ws/game.hpp"
#include <chrono>

int main()
{
    Game game;

    while (!game.getWindow()->isDone())
    {
        auto start = std::chrono::high_resolution_clock::now();
        game.handleInput();
        game.update();
        game.render();
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
        // std::cout << "Game Cycle: " << duration.count() << " microseconds" << std::endl;
    }

    return 0;
}