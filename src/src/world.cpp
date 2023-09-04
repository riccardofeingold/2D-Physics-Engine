#include "../include/no_gravity_game_ws/world.hpp"

using namespace Physics2D;

World2D::World2D()
{
    this->gravity_ = Vector2f(0, 9.81f);
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

void World2D::resolveCollision(const CollisionManifold& collision)
{
    Vector2f v_diff = collision.body_b->getLinearVelocity() - collision.body_a->getLinearVelocity();

    // only apply impulse if the object are moving apart
    if (Math2D::dot(v_diff, collision.normal) > 0.f)
        return;

    float e = std::min(collision.body_a->restitution, collision.body_b->restitution);
    float j = -(1 + e) * Math2D::dot(v_diff, collision.normal);
    j /= collision.body_a->inverse_mass + collision.body_b->inverse_mass;
    
    Vector2f impulse = collision.normal * j;

    collision.body_a->setLinearVelocity(collision.body_a->getLinearVelocity() - impulse * collision.body_a->inverse_mass);
    collision.body_b->setLinearVelocity(collision.body_b->getLinearVelocity() + impulse * collision.body_b->inverse_mass);
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

void World2D::update(const sf::Time& dt, int substeps)
{
    substeps = Math2D::clip(substeps, this->min_substeps, this->max_substeps);
    this->contact_points.clear();
    
    for (int step = 0; step < substeps; ++step)
    {
        // Movement step
        for (int i = 0; i < this->rigidbodies_.size(); ++i)
        {
            this->rigidbodies_[i]->update(dt, this->gravity_, substeps);
        }

        // clearing contacts list
        this->contacts_.clear();

        // collision step
        Vector2f normal;
        float depth;
        for (int i = 0; i < this->rigidbodies_.size() - 1; ++i)
        {
            Rigidbody*& body_a = this->rigidbodies_[i];

            for (int j = i + 1; j < this->rigidbodies_.size(); ++j)
            {
                Rigidbody*& body_b = this->rigidbodies_[j];
                
                if (body_a->is_static && body_b->is_static)
                    continue;
                
                // AABB collision checking
                if (!Collision2D::intersectAABBs(body_a->getAABB(), body_b->getAABB()))
                    continue;

                // SAT collision checking
                if (Collision2D::collide(body_a, body_b, normal, depth))
                {
                    Vector2f penetration = normal * depth;
                    if (body_a->is_static)
                    {
                        body_b->move(penetration);
                    } else if (body_b->is_static)
                    {
                        body_a->move(-penetration);
                    } else
                    {
                        body_a->move(-penetration / 2.f);
                        body_b->move(penetration / 2.f);
                    }

                    Vector2f contact_one = Vector2f::Zero();
                    Vector2f contact_two = Vector2f::Zero();
                    int contact_count = 0;

                    Collision2D::findContactPoint(body_a, body_b, contact_one, contact_two, contact_count);

                    CollisionManifold collision = CollisionManifold(body_a, body_b, normal, depth, contact_one, contact_two, contact_count);
                    this->contacts_.push_back(collision);
                }
            }
        }

        for (int i = 0; i < this->contacts_.size(); ++i)
        {
            CollisionManifold contact = this->contacts_[i];
            this->resolveCollision(this->contacts_[i]);

            if (contact.contact_count > 0)
            {
                if (std::find<std::vector<CollisionManifold>::iterator, CollisionManifold>(this->contacts_.begin(), this->contacts_.end(), contact) != this->contacts_.end())
                    this->contact_points.push_back(contact.contact_one);

                if (contact.contact_count > 1)
                    this->contact_points.push_back(contact.contact_two);
            }
            
        }

    }
    
    // reset forces
    for (int i = 0; i < this->rigidbodies_.size(); ++i)
    {
        this->rigidbodies_[i]->setForce(Vector2f::Zero());
    }
}