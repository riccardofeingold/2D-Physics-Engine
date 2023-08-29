#include "../include/no_gravity_game_ws/vector2dconverter.hpp"

using namespace Physics2D;

sf::Vector2f Vector2Converter::toSFVector2f(Vector2f& a)
{
    return sf::Vector2f(a.x, a.y);
}

Vector2f Vector2Converter::toPhysics2DVector2f(sf::Vector2f& a)
{
    return Vector2f(a.x, a.y);
}