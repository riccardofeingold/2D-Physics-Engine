#include "../include/no_gravity_game_ws/goal.hpp"

Goal::Goal(float height, float width, float screen_height, float screen_width)
    : player_reached_it_(false),
      width_(width),
      height_(height),
      screen_height_(screen_height),
      screen_width_(screen_width)
{
}

Goal::~Goal() = default;

void Goal::setRandomPosition()
{
    body_.setPosition(std::rand()/RAND_MAX*screen_width_, std::rand()/RAND_MAX*screen_height_);
}