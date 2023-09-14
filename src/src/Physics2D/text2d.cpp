#include "../../include/no_gravity_game_ws/Physics2D/text2d.hpp"

using namespace Physics2D;

Text2D::Text2D(const std::string& text, const Vector2f& position, const float scale, const std::string& path_to_font_file = "/Users/riccardofeingold/opt/GameCreations/no_gravity_game_ws/fonts/Roboto-Light.ttf", const int font_size = 20, const sf::Color& color = sf::Color::White, const sf::Text::Style& style = sf::Text::Regular)
{
    this->text_ = text;
    this->position_ = position;
    this->path_to_font_ = path_to_font_file;
    this->font_size_ = font_size;
    this->color_ = color;
    this->style_ = style;
    this->scale_ = scale;
}

Text2D::~Text2D() = default;

void Text2D::draw(Window& window)
{
    sf::Font font;
    if (!font.loadFromFile(this->path_to_font_))
        std::cout << "Couldn't find font!" << std::endl;

    sf::Text step_time_text;
    step_time_text.setString(this->text_);
    step_time_text.setStyle(this->style_);
    step_time_text.setFillColor(this->color_);
    step_time_text.setFont(font);
    step_time_text.setCharacterSize(this->font_size_);
    step_time_text.setScale(this->scale_, this->scale_);
    step_time_text.setPosition(this->position_.x, this->position_.y);
    window.draw(step_time_text);
}