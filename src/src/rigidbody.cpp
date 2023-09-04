#include "../include/no_gravity_game_ws/rigidbody.hpp"

using namespace Physics2D;

Rigidbody::Rigidbody()
{
    this->color = Rigidbody::randomColor();
}

Rigidbody::~Rigidbody() = default;

Rigidbody::Rigidbody(std::string name, const Vector2f& position, float density, float mass, float restitution, float area, bool is_static, float radius, float width, float height, ShapeType shape_type, bool apply_gravity)
{
    this->name = name;

    this->color = Rigidbody::randomColor();

    this->apply_gravity_ = apply_gravity;
    this->force_ = Vector2f::Zero();

    this->position_ = position;
    this->linear_velocity_ = Vector2f::Zero();
    this->rotation_ = 0.f;
    this->angular_velocity_ = 0.f;

    this->density = density;
    this->mass = mass;

    if (!is_static)
        this->inverse_mass = 1/mass;
    else
        this->inverse_mass = 0;

    this->restitution = restitution;
    this->area = area;
    
    this->is_static = is_static;

    this->radius = radius;
    this->width = width;
    this->height = height;

    this->shape_type = shape_type;

    if (this->shape_type == ShapeType::Box)
    {
        this->vertices_ = Rigidbody::createBoxVertices(this->width, this->height);
        this->triangle_indices = Rigidbody::createBoxTriangleIndices();
        this->transformed_vertices_ = std::vector<Vector2f>(this->vertices_.size());
    }

    this->transform_update_required_ = true;
    this->aabb_update_required_ = true;
}

bool Rigidbody::createCircleBody(std::string name, float radius, Vector2f position, float density, bool is_static, float restitution, Rigidbody*& body, std::string& error_message, bool apply_gravity)
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
    body = new Rigidbody(name, position, density, mass, restitution, area, is_static, radius, 0, 0, ShapeType::Circle, apply_gravity);

    return true;
}

bool Rigidbody::createBoxBody(std::string name, float width, float height, Vector2f position, float density, bool is_static, float restitution, Rigidbody*& body, std::string& error_message, bool apply_gravity)
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
    body = new Rigidbody(name, position, density, mass, restitution, area, is_static,0.f, width, height, ShapeType::Box, apply_gravity);

    return true;
}

void Rigidbody::move(const Vector2f& move)
{
    this->position_ += move;
    this->transform_update_required_ = true;
    this->aabb_update_required_ = true;
}

void Rigidbody::moveTo(const Vector2f& move)
{
    this->position_ = move;
    this->transform_update_required_ = true;
    this->aabb_update_required_ = true;
}

void Rigidbody::rotate(const float amount)
{
    this->rotation_ += amount;
    this->transform_update_required_ = true;
    this->aabb_update_required_ = true;
}

void Rigidbody::rotateTo(const float rotation)
{
    // the negations is due to the fact that the z axis is pointing into the screen
    this->rotation_ = rotation;
    this->transform_update_required_ = true;
    this->aabb_update_required_ = true;
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

void Rigidbody::update(const sf::Time& dt, const Vector2f& gravity, int substeps)
{
    if (this->is_static)
        return;

    float time = (float)dt.asSeconds() / substeps;
    
    if (this->apply_gravity_)
        this->linear_velocity_ += gravity * time;

    this->linear_velocity_ += this->force_ / this->mass * time;
    this->position_ += this->linear_velocity_ * time;
    this->rotation_ += this->angular_velocity_ * time;
    
    // this->force_ = Vector2f::Zero();
    this->transform_update_required_ = true;
    this->aabb_update_required_ = true;
}

void Rigidbody::applyForce(const Vector2f& force)
{
    this->force_ = force;
}

// getters
Vector2f Rigidbody::getForce() const { return this->force_; }
Vector2f Rigidbody::getPosition() const { return this->position_; }
Vector2f Rigidbody::getLinearVelocity() const { return this->linear_velocity_; }
float Rigidbody::getRotation() const { return this->rotation_; }
float Rigidbody::getAngularVelocity() const { return this->angular_velocity_; }

AABB Rigidbody::getAABB()
{
    if (this->aabb_update_required_)
    {
        
        float min_x = INFINITY;
        float min_y = INFINITY;
        float max_x = -INFINITY;
        float max_y = -INFINITY;

        if (this->shape_type == ShapeType::Circle)
        {
            min_x = this->position_.x - this->radius;
            min_y = this->position_.y - this->radius;
            max_x = this->position_.x + this->radius;
            max_y = this->position_.y + this->radius;   
        } else if (this->shape_type == ShapeType::Box)
        {
            std::vector<Vector2f> vertices = this->getTransformedVertices();

            for (int i = 0; i < vertices.size(); ++i)
            {
                Vector2f v = vertices[i];
                if (v.x < min_x)
                    min_x = v.x;
                if (v.y < min_y)
                    min_y = v.y;
                if (v.x > max_x)
                    max_x = v.x;
                if (v.y > max_y)
                    max_y = v.y;
            }
        } else
        {
            std::cout << "ERROR" << std::endl;
        }

        this->aabb_ = AABB(min_x, min_y, max_x, max_y);
    }

    this->aabb_update_required_ = false;
    return this->aabb_;
}

// setters
void Rigidbody::setLinearVelocity(const Vector2f& value) { this->linear_velocity_ = value; }
void Rigidbody::setAngularVelocity(const float value) { this->angular_velocity_ = value; }
void Rigidbody::setForce(const Vector2f& value) { this->force_ = value; }