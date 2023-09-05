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
    if (!Rigidbody::createCircleBody(radius, 1.f, is_static, 1.f, 0.6f, 0.4f, body, e, true))
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
    if (!Rigidbody::createBoxBody(width, height, 1.f, is_static, 1.f, 0.6f, 0.4f, body, e, true))
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
        sf::ConvexShape line(4);
        Vector2f va = Vector2f::Zero();
        Vector2f vb = Vector2f(this->body->radius, 0);
        Vector2f vc = Vector2f(this->body->radius, 0.01f);
        Vector2f vd = Vector2f(0.f, 0.01f);

        Vector2f position = this->body->getPosition();
        Transform2D transform = Transform2D(position, this->body->getRotation());

        va = Vector2f::transform(va, transform);
        vb = Vector2f::transform(vb, transform);
        vc = Vector2f::transform(vc, transform);
        vd = Vector2f::transform(vd, transform);

        line.setPoint(0, Vector2Converter::toSFVector2f(va));
        line.setPoint(1, Vector2Converter::toSFVector2f(vb));
        line.setPoint(2, Vector2Converter::toSFVector2f(vc));
        line.setPoint(3, Vector2Converter::toSFVector2f(vd));

        line.setOutlineThickness(0.1f);
        line.setOutlineColor(sf::Color::White);

        sf::CircleShape circle(this->body->radius);
        circle.setFillColor(this->color);
        circle.setOutlineColor(sf::Color::White);
        circle.setOutlineThickness(0.1f);
        circle.setOrigin(circle.getRadius(), circle.getRadius());
        circle.setPosition(pos);
        window.draw(circle);
        window.draw(line);
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

        window.draw(box);
    }
}

bool Entity2D::operator==(const Entity2D& object)
{
    return this->body == object.body &&
           this->color == object.color &&
           this->name == object.name;
}