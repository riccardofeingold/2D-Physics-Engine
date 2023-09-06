#include "../../include/no_gravity_game_ws/Physics2D/vector2dconverter.hpp"

using namespace Physics2D;

sf::Vector2f Vector2Converter::toSFVector2f(const Vector2f& a)
{
    return sf::Vector2f(a.x, a.y);
}

Vector2f Vector2Converter::toPhysics2DVector2f(const sf::Vector2f& a)
{
    return Vector2f(a.x, a.y);
}

void Vector2Converter::toSFVector2fList(const std::vector<Vector2f>& src, std::vector<sf::Vector2f>& dst)
{
    if (dst.empty() || src.size() != dst.size())
        dst = std::vector<sf::Vector2f>(src.size());

    for (int i = 0; i < dst.size(); ++i)
    {
        dst[i].x = src[i].x;
        dst[i].y = src[i].y;
    }
}

void Vector2Converter::toPhysics2DVector2fList(const std::vector<sf::Vector2f>& src, std::vector<Vector2f>& dst)
{
    if (dst.empty() || src.size() != dst.size())
        dst = std::vector<Vector2f>(src.size());

    for (int i = 0; i < dst.size(); ++i)
    {
        dst[i].x = src[i].x;
        dst[i].y = src[i].y;
    }
}