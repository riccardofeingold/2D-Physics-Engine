#ifndef GAME_HPP
#define GAME_HPP

#include "window.hpp"
#include "player.hpp"
#include "physics.hpp"
#include "world.hpp"
#include "goal.hpp"
#include "enemy.hpp"
#include "wall.hpp"
#include "rigidbody.hpp"
#include "vector2dconverter.hpp"
#include "collision2d.hpp"
#include <SFML/Graphics.hpp>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SPEED 200
#define SPEED_JOYSTICK 0.02 // maximum value of the joystick is 100
#define MAX_THRUST_FORCE 0.5
#define MAX_ROTATIONAL_TORQUE 1
#define GRAVITY 10
#define FRAME_RATE 60
#define NUM_WALLS 0
#define MAX_WALL_SIZE 100

// air drag values
#define DRAG_COEFFICIENT 1.05 // https://en.wikipedia.org/wiki/Drag_coefficient
#define AIR_DENSITY 1.293

using namespace Physics2D;

class Game
{
    public:
    // constuctors
    Game();

    // deconstructors
    ~Game();

    // methods
    void handleInput();
    void start();
    void update();
    void render();
    Window* getWindow();

    // public variables
    sf::View view; 

    private:
    sf::Clock clock_;
    sf::Time dt_;
    Window window_;
    World2D world_;
    // Player player_;
    // Goal goal_;
    // Enemy enemy_;
    // Enemy tester_;
    // std::vector<Wall> walls_;

    std::vector<sf::Vector2f> vertex_buffer_;

};

#endif