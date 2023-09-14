#ifndef TEXT_2D_HPP
#define TEXT_2D_HPP

#include "vector2d.hpp"
#include "window.hpp"
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

namespace Physics2D
{
    class Text2D
    {
        public:
        Text2D(const std::string& text, const Vector2f& position, const float scale, const std::string& path_to_font_file, const int font_size, const sf::Color& color, const sf::Text::Style& style);
        ~Text2D();

        void draw(Window& window);

        private:
        std::string text_;
        Vector2f position_;
        std::string path_to_font_;
        int font_size_;
        sf::Color color_;
        sf::Text::Style style_;
        float scale_;
    };
} // namespace Physics2D


#endif