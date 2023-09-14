#include "../../include/no_gravity_game_ws/Physics2D/point2d.hpp"

using namespace Physics2D;

Point2D::Point2D(const float radius, const sf::Color& color, const Vector2f& position)
{
    this->radius_ = radius;
    this->color_ = color;
    this->position_ = position;
}

Point2D::~Point2D() = default;

void Point2D::draw(Window& window)
{
    // draw a circle
    sf::CircleShape point(this->radius_);

    point.setFillColor(this->color_);
    point.setOrigin(this->radius_, this->radius_);
    point.setPosition(this->position_.x,this->position_.y);
    window.draw(point);
}