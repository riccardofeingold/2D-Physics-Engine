#include "../include/no_gravity_game_ws/player.hpp"

Player::Player(float x, float y, float size, float restitution)
{
    this->on_ground = true;
    this->size = size;
    this->restitution = restitution;

    // player initialization
    this->velocity_ = sf::Vector2f(0.f, 0.f);
    this->body = sf::CircleShape(size);
    this->body.setFillColor(sf::Color::White);
    this->body.setPosition(x - size, y - size);
}

Player::~Player() = default;

// setters
void Player::setVelocity(const sf::Vector2f& velocity) { this->velocity_ = velocity; }

// getters
const sf::Vector2f& Player::getVelocity() const { return this->velocity_; }

void Player::move(const sf::Vector2f& position)
{
    this->body.move(position);
}

void Player::shoot()
{

}