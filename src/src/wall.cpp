#include "../include/no_gravity_game_ws/wall.hpp"

Wall::Wall(const sf::Vector2f& position, const sf::Vector2f& size, float rotation)
{
    this->size_ = size;
    this->material_.restitution = 1;

    this->body_ = sf::RectangleShape(size_);
    this->body_.setFillColor(Colors().WALL);
    this->body_.setOrigin(size_.x/2, size_.y/2);
    std::cout << position.x << " " << position.y << std::endl;
    this->body_.setPosition(position);
    this->body_.setRotation(rotation);
}

Wall::~Wall() = default;

void Wall::update(sf::Time delta_time)
{

}

void Wall::move()
{

}

void Wall::reset()
{

}