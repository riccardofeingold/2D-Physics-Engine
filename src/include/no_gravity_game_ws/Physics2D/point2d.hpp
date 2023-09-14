#ifndef POINT_2D_HPP
#define POINT_2D_HPP

#include "vector2d.hpp"
#include "window.hpp"
#include <SFML/Graphics.hpp>

namespace Physics2D
{
    class Point2D
    {
        public:
        Point2D(const float radius, const sf::Color& color, const Vector2f& position);
        ~Point2D();

        void draw(Window& window);

        private:
        float radius_;
        sf::Color color_;
        Vector2f position_;
    };
} // namespace Physics2D


#endif