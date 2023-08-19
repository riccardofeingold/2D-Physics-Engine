#include "../include/no_gravity_game_ws/goal.hpp"

Goal::Goal(sf::Vector2f size, float restitution = 0.f)
{
    this->size_ = size;
    this->material_.restitution = restitution;
    this->body_ = sf::RectangleShape(sf::Vector2f(20, 20));
    this->body_.setFillColor(sf::Color::Green);
    this->setRandomPosition();
}

Goal::~Goal() = default;

void Goal::setRandomPosition()
{
    sf::Vector2f random_position = sf::Vector2f((float)std::rand()/RAND_MAX*this->window_->getWindowSize().x, (float)std::rand()/RAND_MAX*this->window_->getWindowSize().y);
    this->body_.setPosition(random_position);
}

void Goal::reset()
{
    this->acceleration_ = sf::Vector2f(0, 0);
    this->velocity_ = sf::Vector2f(0, 0);
    this->position_ = sf::Vector2f(0, 0);
    this->orientation_ = 0;
    this->angular_velocity_ = 0;
    this->angular_acceleration_ = 0;
    this->setRandomPosition();
}

void Goal::move()
{
    // setting origin always to the center
    this->body_.setOrigin(this->size_.x/2, this->size_.y/2);
}