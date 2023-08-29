#include "../include/no_gravity_game_ws/vector2d.hpp"

Physics2D::Vector2f::Vector2f(float x, float y)
{
    this->x = x;
    this->y = y;
}

Physics2D::Vector2f Physics2D::Vector2f::Zero()
{
    return Physics2D::Vector2f(0.f, 0.f);
}

Physics2D::Vector2f Physics2D::Vector2f::operator+(Physics2D::Vector2f& a)
{
    return Physics2D::Vector2f(this->x + a.x, this->y + a.y);
}

Physics2D::Vector2f Physics2D::Vector2f::operator-(Physics2D::Vector2f& a)
{
    return Physics2D::Vector2f(this->x - a.x, this->y - a.y);
}

Physics2D::Vector2f Physics2D::Vector2f::operator-()
{
    return Physics2D::Vector2f(-this->x, -this->y);
}

Physics2D::Vector2f Physics2D::Vector2f::operator*(float scalar)
{
    return Physics2D::Vector2f(this->x * scalar, this->y * scalar);
}

Physics2D::Vector2f Physics2D::Vector2f::operator/(float scalar)
{
    return Physics2D::Vector2f(this->x / scalar, this->y / scalar);
}

bool Physics2D::Vector2f::operator==(Physics2D::Vector2f& a)
{
    return this->x == a.x && this->y == a.y;
}

float Physics2D::Vector2f::dot(Physics2D::Vector2f& a)
{
    return this->x * a.x + this->y * a.y;
}

std::size_t Physics2D::Vector2f::getHashCode()
{
    return std::hash<Physics2D::Vector2f>{}(Vector2f(this->x, this->y));
}

std::string Physics2D::Vector2f::toString()
{
    return "X: " + std::to_string(this->x) + " Y: " + std::to_string(this->y);
}