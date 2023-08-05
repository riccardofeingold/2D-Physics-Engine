#include "include/player.hpp"

Player::Player(float x, float y, float size)
{
    this->on_ground_ = true;

    // player initialization
    this->shape_ = sf::CircleShape(size);
    this->shape_.setFillColor(sf::Color::White);
    this->shape_.setPosition(x + size, y - size);
}

void Player::jump()
{
    this->on_ground_ = false;
    // simulate upwards motion
}