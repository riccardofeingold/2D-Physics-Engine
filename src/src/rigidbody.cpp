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

    if (this->shape_type_ == ShapeType::Box)
    {
        this->vertices_ = Rigidbody::createBoxVertices(this->width_, this->height_);
        this->triangle_indices = Rigidbody::createBoxTriangleIndices();
        this->transformed_vertices_ = std::vector<Vector2f>(this->vertices_.size());
    }

    this->transform_update_required_ = true;
}

bool Rigidbody::createCircleBody(float radius, Vector2f position, float density, bool is_static, float restitution, Rigidbody*& body, std::string& error_message)
{
    error_message = "";
    float area = M_PI*radius*radius;

    if (area < World2D::min_body_size)
    {
        body = nullptr;
        error_message = "Circle body is too small. The min body size is " + std::to_string(World2D::min_body_size);
        return false;
    }

    if (area > World2D::max_body_size)
    {
        body = nullptr;
        error_message = "Circle body is too big. The max body size is " + std::to_string(World2D::max_body_size);
        return false;
    }

    if (density < World2D::min_density)
    {
        body = nullptr;
        error_message = "The circle's density is too small. The min density is " + std::to_string(World2D::min_density);
        return false;
    }

    if (density > World2D::max_density)
    {
        body = nullptr;
        error_message = "The circle's density is too big. The max density is " + std::to_string(World2D::max_density);
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

    if (area < World2D::min_body_size)
    {
        body = nullptr;
        error_message = "Box body is too small. The min body size is " + std::to_string(World2D::min_body_size);
        return false;
    }

    if (area > World2D::max_body_size)
    {
        body = nullptr;
        error_message = "Box body is too big. The max body size is " + std::to_string(World2D::max_body_size);
        return false;
    }

    if (density < World2D::min_density)
    {
        body = nullptr;
        error_message = "The box's density is too small. The min density is " + std::to_string(World2D::min_density);
        return false;
    }

    if (density > World2D::max_density)
    {
        body = nullptr;
        error_message = "The box's density is too big. The max density is " + std::to_string(World2D::max_density);
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
    this->transform_update_required_ = true;
}

void Rigidbody::moveTo(const Vector2f& move)
{
    this->position_ = move;
    this->transform_update_required_ = true;
}

void Rigidbody::rotate(const float amount)
{
    this->rotation_ += amount;
    this->transform_update_required_ = true;
}

void Rigidbody::rotateTo(const float rotation)
{
    // the negations is due to the fact that the z axis is pointing into the screen
    this->rotation_ = rotation;
    this->transform_update_required_ = true;
}

sf::Color Rigidbody::randomColor()
{
    uint8_t r = (float)std::rand()/RAND_MAX*255;
    uint8_t g = (float)std::rand()/RAND_MAX*255;
    uint8_t b = (float)std::rand()/RAND_MAX*255;

    return sf::Color(r, g, b);
}

std::vector<Vector2f> Rigidbody::createBoxVertices(float width, float height)
{
    float left = -width / 2.f;
    float right = width / 2.f;
    float bottom = height / 2.f;
    float top = -height / 2.f;

    std::vector<Vector2f> vertices(4);
    vertices[0] = Vector2f(left, top);
    vertices[1] = Vector2f(right, top);
    vertices[2] = Vector2f(right, bottom);
    vertices[3] = Vector2f(left, bottom);

    return vertices;
}

std::vector<int> Rigidbody::createBoxTriangleIndices()
{
    // we go clock wise starting at the top left corner
    std::vector<int> triangles(6);

    triangles[0] = 0;
    triangles[1] = 1;
    triangles[2] = 2;
    triangles[3] = 0;
    triangles[4] = 2;
    triangles[5] = 3;

    return triangles;
}

const std::vector<Vector2f>& Rigidbody::getTransformedVertices()
{
    if (this->transform_update_required_)
    {
        for (int i = 0; i < this->vertices_.size(); ++i)
        {
            Transform2D transform = Transform2D(this->position_, this->rotation_);
            Vector2f transformed_vertex_pos = Vector2f::transform(this->vertices_[i], transform);
            this->transformed_vertices_[i] = transformed_vertex_pos;
        }
    }

    this->transform_update_required_ = false;
    return this->transformed_vertices_;
}

void Rigidbody::update(const sf::Time& dt)
{
    this->position_ += this->linear_velocity_ * dt.asSeconds();
    this->rotation_ += this->angular_velocity_ * dt.asSeconds();
}