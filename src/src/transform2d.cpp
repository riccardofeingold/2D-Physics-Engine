#include "../include/no_gravity_game_ws/transform2d.hpp"

using namespace Physics2D;

Transform2D::Transform2D(Vector2f& position, float angle)
{
    this->position_x = position.x;
    this->position_y = position.y;
    
    float radians = angle*M_PI/180;
    this->sin = std::sin(radians);
    this->cos = std::cos(radians);
}

Transform2D::Transform2D(float x, float y, float angle)
{
    this->position_x = x;
    this->position_y = y;

    float radians = angle*M_PI/180;
    this->sin = std::sin(radians);
    this->cos = std::cos(radians);
}

Transform2D Transform2D::Zero()
{
    return Transform2D(0.f, 0.f, 0.f);
}