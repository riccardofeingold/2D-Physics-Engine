#include "../include/no_gravity_game_ws/player.hpp"

Player::Player(float x, float y, float size, float restitution)
{
    this->on_ground = true;
    this->size = size;
    this->restitution = restitution;

    // player initialization
    this->velocity_ = sf::Vector2f(0.f, 0.f);
    this->body_ = sf::CircleShape(size);
    this->body_.setFillColor(sf::Color::White);
    this->body_.setPosition(x - size, y - size);
}

Player::~Player() = default;

// setters
void Player::setVelocity(const sf::Vector2f& velocity) { this->velocity_ = velocity; }

// getters
const sf::Vector2f& Player::getVelocity() const { return this->velocity_; }

void Player::move()
{
    this->body_.move(this->position_);
}

void Player::move(const sf::Vector2f& position)
{
    this->body_.move(position);
}

void Player::shoot()
{

}