#include "../include/no_gravity_game_ws/math2d.hpp"

using namespace Physics2D;

Math2D::Math2D()
{

}

float Math2D::norm(Vector2f& a)
{
    return std::sqrt(a.x * a.x + a.y * a.y);
}

float Math2D::distance(Vector2f& a, Vector2f& b)
{
    return std::sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

Vector2f Math2D::normalize(Vector2f& a)
{
    return a / Math2D::norm(a);
}

float Math2D::dot(Vector2f& a)
{
    return a.x * a.x + a.y * a.y;
}

float Math2D::cross(Vector2f& a, Vector2f& b)
{
    return a.x * b.y - a.y * b.x;
}