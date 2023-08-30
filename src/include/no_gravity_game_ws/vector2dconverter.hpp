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

        static sf::Vector2f toSFVector2f(Vector2f& a);
        static Vector2f toPhysics2DVector2f(sf::Vector2f& a);
    };
} // namespace Physics2D


#endif