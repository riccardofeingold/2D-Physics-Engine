#include "../include/no_gravity_game_ws/entity2d.hpp"

using namespace Physics2D;

Entity2D::Entity2D(const std::string& name, Rigidbody*& body, const sf::Color& color, World2D& world)
{
    this->name = name;
    this->body = body;
    this->color = color;

    world.addObject(this->name, this->body);
}

Entity2D::Entity2D(const std::string& name, Rigidbody*& body, World2D& world)
{
    this->name = name;
    this->body = body;
    this->color = Rigidbody::randomColor();

    world.addObject(this->name, this->body);
}

Entity2D::Entity2D(const std::string& name, const float radius, const bool is_static, const Vector2f& position, World2D& world)
{
    Rigidbody* body;
    std::string e;
    if (!Rigidbody::createCircleBody(radius, 1.f, is_static, 1.f, body, e, true))
    {
        std::cout << e << std::endl;
        assert(1 == 0);
    }

    this->body = body;
    this->name = name;
    this->color = Rigidbody::randomColor();
    this->body->moveTo(position);
    world.addObject(this->name, this->body);
}

Entity2D::Entity2D(const std::string& name, const float width, const float height, const bool is_static, const Vector2f& position, World2D& world)
{
    Rigidbody* body;
    std::string e;
    if (!Rigidbody::createBoxBody(width, height, 1.f, is_static, 1.f, body, e, true))
    {
        std::cout << e << std::endl;
        assert(1 == 0);
    }

    this->body = body;
    this->name = name;
    this->color = Rigidbody::randomColor();
    this->body->moveTo(position);
    world.addObject(name, body);
}

void Entity2D::draw(Window& window)
{
    sf::Vector2f pos = Vector2Converter::toSFVector2f(this->body->getPosition());
    if (this->body->shape_type == ShapeType::Circle)
    {
        sf::CircleShape circle(this->body->radius);
        circle.setFillColor(this->color);
        circle.setOutlineColor(sf::Color::White);
        circle.setOutlineThickness(0.1f);
        circle.setOrigin(circle.getRadius(), circle.getRadius());
        circle.setPosition(pos);
        window.draw(circle);
    } else if (this->body->shape_type == ShapeType::Box)
    {
        Vector2Converter::toSFVector2fList(this->body->getTransformedVertices(), this->vertex_buffer);

        sf::ConvexShape box(4);
        for (int i = 0; i < box.getPointCount(); ++i)
        {
            box.setPoint(i, this->vertex_buffer[i]);
        }

        box.setFillColor(this->color);
        box.setOutlineThickness(0.1f);
        box.setOrigin(pos);
        box.setPosition(pos);

        window.draw(box);
    }
}

bool Entity2D::operator==(const Entity2D& object)
{
    return this->body == object.body &&
           this->color == object.color &&
           this->name == object.name;
}