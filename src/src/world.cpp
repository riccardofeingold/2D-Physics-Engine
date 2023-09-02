#include "../include/no_gravity_game_ws/world.hpp"

using namespace Physics2D;

World2D::World2D()
{
}

World2D::~World2D() = default;

void World2D::reset()
{
}

void World2D::addObject(const std::string& name, Rigidbody* obj)
{
    this->rigidbodies_.push_back(obj);
    this->list_of_object_names_.push_back(name);
}

bool World2D::removeObject(const std::string& name)
{
    std::vector<std::string>::iterator it = std::find(this->list_of_object_names_.begin(), this->list_of_object_names_.end(), name);
    if (it != this->list_of_object_names_.end())
    {
        int index = std::distance(this->list_of_object_names_.begin(), it);
        this->list_of_object_names_.erase(it);

        std::vector<Rigidbody*>::iterator it2 = this->rigidbodies_.begin();
        std::advance(it2, index);
        this->rigidbodies_.erase(it2);
        
        return true;
    }

    return false;
}

void World2D::resolveCollision(Rigidbody*& body_a, Rigidbody*& body_b, const Vector2f& normal, const float& depth)
{
    float e = std::min(body_a->restitution, body_b->restitution);
    Vector2f v_diff = body_b->getLinearVelocity() - body_a->getLinearVelocity();
    float j_nom = -(1 + e) * Math2D::dot(v_diff, normal);
    float j_den = 1.f/body_a->mass + 1.f/body_b->mass;
    float j = j_nom / j_den; 

    body_a->setLinearVelocity(body_a->getLinearVelocity() - normal * j / body_a->mass);
    body_b->setLinearVelocity(body_b->getLinearVelocity() + normal * j / body_b->mass);
}

bool World2D::getBody(int index, Rigidbody*& body)
{
    if (index < 0 || index >= this->rigidbodies_.size())
    {
        return false;
    }

    body = this->rigidbodies_[index];
    return true;
}

bool World2D::getBody(std::string name, Rigidbody*& body)
{
    std::vector<std::string>::iterator it = std::find(this->list_of_object_names_.begin(), this->list_of_object_names_.end(), name);
    if (it != this->list_of_object_names_.end())
    {
        body = this->rigidbodies_[std::distance(this->list_of_object_names_.begin(), it)];
        return true;
    }
    
    return false;
}

int World2D::getBodyCount() const
{
    return this->rigidbodies_.size();
}

void World2D::update(const sf::Time& dt)
{
    // Movement step
    for (int i = 0; i < this->rigidbodies_.size(); ++i)
    {
        this->rigidbodies_[i]->update(dt);
    }

    // collision step
    Vector2f normal;
    float depth;
    for (int i = 0; i < this->rigidbodies_.size() - 1; ++i)
    {
        Rigidbody* body_a = this->rigidbodies_[i];

        for (int j = i + 1; j < this->rigidbodies_.size(); ++j)
        {
            Rigidbody* body_b = this->rigidbodies_[j];

            if (this->collide(body_a, body_b, normal, depth))
            {
                body_a->move(-normal * depth / 2.f);
                body_b->move(normal * depth / 2.f);
                this->resolveCollision(body_a, body_b, normal, depth);
            }
        }
    }
}

bool World2D::collide(Rigidbody* body_a, Rigidbody* body_b, Vector2f& normal, float& depth)
{
    normal = Vector2f::Zero();
    depth = 0.f;

    ShapeType shape_type_a = body_a->shape_type;
    ShapeType shape_type_b = body_b->shape_type;

    if (shape_type_a == ShapeType::Box)
    {
        if (shape_type_b == ShapeType::Box)
        {
            return Collision2D::polygonCollisionDetection(body_a->getTransformedVertices(), body_b->getTransformedVertices(), normal, depth);
        } else if (shape_type_b == ShapeType::Circle)
        {
            bool result = Collision2D::circlePolygonCollisionDetection(body_b->getPosition(), body_b->radius, body_a->getTransformedVertices(), normal, depth);
            normal = -normal;
            return result;
        }
    } else if (shape_type_a == ShapeType::Circle)
    {
        if (shape_type_b == ShapeType::Box)
        {
            return Collision2D::circlePolygonCollisionDetection(body_a->getPosition(), body_a->radius, body_b->getTransformedVertices(), normal, depth);   
        } else if (shape_type_b == ShapeType::Circle)
        {
            return Collision2D::circleCollisionDetection(body_a->getPosition(), body_a->radius, body_b->getPosition(), body_b->radius, normal, depth);   
        }
    }

    return false;
}