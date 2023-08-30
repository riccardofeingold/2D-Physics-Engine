#ifndef VECTOR_2D_CONVERTER_HPP
#define VECTOR_2D_CONVERTER_HPP 

#include <SFML/Graphics.hpp>
#include "vector2d.hpp"

namespace Physics2D
{
    class Vector2Converter
    {
        public:
        Vector2Converter() = default;

        static sf::Vector2f toSFVector2f(const Vector2f& a);
        static Vector2f toPhysics2DVector2f(const sf::Vector2f& a);
        static void toSFVector2fList(const std::vector<Vector2f>& a, std::vector<sf::Vector2f>& b);
        static void toPhysics2DVector2fList(const std::vector<sf::Vector2f>& a, std::vector<Vector2f>& b);
    };
} // namespace Physics2D


#endif