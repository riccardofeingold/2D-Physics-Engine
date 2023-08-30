#include "../include/no_gravity_game_ws/rigidbody.hpp"

using namespace Physics2D;

Rigidbody::Rigidbody()
{
    this->color_ = Rigidbody::randomColor();
}

Rigidbody::~Rigidbody() = default;

Rigidbody::Rigidbody(const Vector2f& position, float density, float mass, float restitution, float area, bool is_static, float radius, float width, float height, ShapeType shape_type)
{
    this->color_ = Rigidbody::randomColor();

    this->position_ = position;
    this->linear_velocity_ = Vector2f::Zero();
    this->rotation_ = 0.f;
    this->angular_velocity_ = 0.f;

    this->density_ = density;
    this->mass_ = mass;
    this->restitution_ = restitution;
    this->area_ = area;
    
    this->is_static_ = is_static;

    this->radius_ = radius;
    this->width_ = width;
    this->height_ = height;

    this->shape_type_ = shape_type;
}

bool Rigidbody::createCircleBody(float radius, Vector2f position, float density, bool is_static, float restitution, Rigidbody*& body, std::string& error_message)
{
    error_message = "";
    float area = M_PI*radius*radius;

    if (area < World::min_body_size)
    {
        body = nullptr;
        error_message = "Circle body is too small. The min body size is " + std::to_string(World::min_body_size);
        return false;
    }

    if (area > World::max_body_size)
    {
        body = nullptr;
        error_message = "Circle body is too big. The max body size is " + std::to_string(World::max_body_size);
        return false;
    }

    if (density < World::min_density)
    {
        body = nullptr;
        error_message = "The circle's density is too small. The min density is " + std::to_string(World::min_density);
        return false;
    }

    if (density > World::max_density)
    {
        body = nullptr;
        error_message = "The circle's density is too big. The max density is " + std::to_string(World::max_density);
        return false;
    }

    restitution = Math2D::clip(restitution, 0.f, 1.f);
    float mass = area * density;
    body = new Rigidbody(position, density, mass, restitution, area, is_static, radius, 0, 0, ShapeType::Circle);

    return true;
}

bool Rigidbody::createBoxBody(float width, float height, Vector2f position, float density, bool is_static, float restitution, Rigidbody*& body, std::string& error_message)
{
    error_message = "";
    float area = width*height;

    if (area < World::min_body_size)
    {
        body = nullptr;
        error_message = "Box body is too small. The min body size is " + std::to_string(World::min_body_size);
        return false;
    }

    if (area > World::max_body_size)
    {
        body = nullptr;
        error_message = "Box body is too big. The max body size is " + std::to_string(World::max_body_size);
        return false;
    }

    if (density < World::min_density)
    {
        body = nullptr;
        error_message = "The box's density is too small. The min density is " + std::to_string(World::min_density);
        return false;
    }

    if (density > World::max_density)
    {
        body = nullptr;
        error_message = "The box's density is too big. The max density is " + std::to_string(World::max_density);
        return false;
    }

    restitution = Math2D::clip(restitution, 0.f, 1.f);
    float mass = area * density;
    body = new Rigidbody(position, density, mass, restitution, area, is_static,0.f, width, height, ShapeType::Box);

    return true;
}

void Rigidbody::move(const Vector2f& move)
{
    this->position_ += move;
}

void Rigidbody::moveTo(const Vector2f& move)
{
    this-> position_ = move;
}

sf::Color Rigidbody::randomColor()
{
    uint8_t r = (float)std::rand()/RAND_MAX*255;
    uint8_t g = (float)std::rand()/RAND_MAX*255;
    uint8_t b = (float)std::rand()/RAND_MAX*255;

    return sf::Color(r, g, b);
}