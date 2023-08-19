#include "../include/no_gravity_game_ws/player.hpp"

Player::Player(float x, float y, sf::Vector2f size, float restitution)
{
    this->size_ = size;
    this->material_.restitution = restitution;
    this->initial_position_ = sf::Vector2f(x - size.x/2, y - size.y/2);
    
    // player initialization
    this->moment_of_inertia_ = MomentOfInertia(this->size_.x, this->size_.y, 1.f, this->getMass(), "z");
    this->velocity_ = sf::Vector2f(0.f, 0.f);
    this->body_ = sf::RectangleShape(size);
    this->body_.setFillColor(sf::Color::White);
    this->body_.setPosition(this->initial_position_);
}

Player::~Player() = default;

void Player::move()
{
    this->body_.setOrigin(this->size_.x/2, this->size_.y/2);
    this->body_.move(this->position_);
    this->body_.rotate(this->orientation_);
}

void Player::move(const sf::Vector2f& position)
{
    this->body_.move(position);
}

void Player::reset()
{
    this->acceleration_ = sf::Vector2f(0, 0);
    this->velocity_ = sf::Vector2f(0, 0);
    this->position_ = sf::Vector2f(0, 0);
    this->orientation_ = 0;
    this->angular_velocity_ = 0;
    this->angular_acceleration_ = 0;
    this->body_.setPosition(this->initial_position_);
}

void Player::shoot()
{

}