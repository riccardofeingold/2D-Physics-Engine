#ifndef ENTITY_2D_HPP
#define ENTITY_2D_HPP

#include "rigidbody.hpp"

namespace Physics2D
{   
    class Entity2D
    {
        public:
        Entity2D(const std::string& name, Rigidbody*& body, World2D& world);
        Entity2D(const std::string& name, Rigidbody*& body, const sf::Color& color, World2D& world);
        Entity2D(const std::string& name, const float radius, const bool is_static, const Vector2f& position, World2D& world);
        Entity2D(const std::string& name, const float width, const float height, const bool is_static, const Vector2f& position, World2D& world);
        ~Entity2D() = default;

        void draw(Window& window);
        bool operator==(const Entity2D& object);

        Rigidbody* body;
        sf::Color color;
        std::string name;

        std::vector<sf::Vector2f> vertex_buffer;
    };
} // namespace Physics2D
    

#endif