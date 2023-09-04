#ifndef GAME_HPP
#define GAME_HPP

#include <chrono>

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
#define ZOOM 0.08

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
    std::vector<sf::Vector2f> vertex_buffer_;

    // first stopwatch
    std::chrono::steady_clock::time_point watch_start_;
    std::chrono::steady_clock::time_point watch_stop_;
    std::chrono::microseconds duration_;

    // second stopwatch
    std::chrono::steady_clock::time_point watch2_start_;
    std::chrono::steady_clock::time_point watch2_stop_;
    std::chrono::microseconds duration2_;

    long long total_world_time_step_ = 0;
    int total_body_count_ = 0;
    int total_samples_ = 0;
    std::string time_step_string_ = "";
    std::string body_count_string_ = "";
    // Player player_;
    // Goal goal_;
    // Enemy enemy_;
    // Enemy tester_;
    // std::vector<Wall> walls_;

    // private methods
    /// @brief keeps the objects inside the screen
    void wrapScreen();
};

#endif