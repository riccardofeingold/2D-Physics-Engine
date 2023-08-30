#include "../include/no_gravity_game_ws/vector2d.hpp"

using namespace Physics2D;

Vector2f::Vector2f()
{
    this->x = 0.f;
    this->y = 0.f;
}

Vector2f::Vector2f(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2f Vector2f::Zero()
{
    return Vector2f(0.f, 0.f);
}

Vector2f Vector2f::operator+(const Vector2f& a)
{
    return Vector2f(this->x + a.x, this->y + a.y);
}

Vector2f Vector2f::operator+=(const Vector2f& a)
{
    this->x += a.x;
    this->y += a.y;
    return *this;
}

Vector2f Vector2f::operator-=(const Vector2f& a)
{
    return *this - a;
}

Vector2f Vector2f::operator-(const Vector2f& a)
{
    return Vector2f(this->x - a.x, this->y - a.y);
}

Vector2f Vector2f::operator-()
{
    return Vector2f(-this->x, -this->y);
}

Vector2f Vector2f::operator*(const float scalar)
{
    return Vector2f(this->x * scalar, this->y * scalar);
}

Vector2f Vector2f::operator/(const float scalar)
{
    return Vector2f(this->x / scalar, this->y / scalar);
}

bool Vector2f::operator==(const Vector2f& a)
{
    return this->x == a.x && this->y == a.y;
}

std::size_t Vector2f::getHashCode()
{
    return std::hash<Vector2f>{}(Vector2f(this->x, this->y));
}

std::string Vector2f::toString()
{
    return "X: " + std::to_string(this->x) + " Y: " + std::to_string(this->y);
}