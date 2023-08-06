#include "../include/no_gravity_game_ws/player.hpp"

Player::Player(float x, float y, float size, float restitution)
{
    this->on_ground_ = true;
    this->size = size;
    this->restitution = restitution;

    // player initialization
    this->velocity = sf::Vector2f(0.f, 0.f);
    this->body = sf::CircleShape(size);
    this->body.setFillColor(sf::Color::White);
    this->body.setPosition(x + size, y - size);
}

Player::~Player() = default;

void Player::move()
{

}

void Player::shoot()
{

}

void Player::jump()
{
    this->on_ground_ = false;
    // simulate upwards motion
}