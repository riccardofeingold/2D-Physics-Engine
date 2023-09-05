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
    this->list_of_object_names.push_back(name);
}

bool World2D::removeObject(const std::string& name)
{
    std::vector<std::string>::iterator it = std::find(this->list_of_object_names.begin(), this->list_of_object_names.end(), name);
    if (it != this->list_of_object_names.end())
    {
        int index = std::distance(this->list_of_object_names.begin(), it);
        this->list_of_object_names.erase(it);

        std::vector<Rigidbody*>::iterator it2 = this->rigidbodies_.begin();
        std::advance(it2, index);
        this->rigidbodies_.erase(it2);
        
        return true;
    }

    return false;
}

void World2D::resolveCollisionBasic(const CollisionManifold& collision)
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

void World2D::resolveCollisionWithRotation(const CollisionManifold& collision)
{
    Rigidbody* body_a = collision.body_a;
    Rigidbody* body_b = collision.body_b;
    Vector2f normal = collision.normal;
    Vector2f contact_one = collision.contact_one;
    Vector2f contact_two = collision.contact_two;
    int contact_count = collision.contact_count;

    float e = std::min(body_a->restitution, body_b->restitution);

    this->contact_list_[0] = contact_one;
    this->contact_list_[1] = contact_two;

    for (int i = 0; i < contact_count; ++i)
    {
        this->impulse_list_[i] = Vector2f::Zero();
        this->ra_list_[i] = Vector2f::Zero();
        this->rb_list_[i] = Vector2f::Zero();
    }

    for (int i = 0; i < contact_count; ++i)
    {
        Vector2f ra = this->contact_list_[i] - body_a->getPosition();
        Vector2f rb = this->contact_list_[i] - body_b->getPosition();

        this->ra_list_[i] = ra;
        this->rb_list_[i] = rb;
        
        Vector2f ra_perp = Vector2f(-ra.y, ra.x);
        Vector2f rb_perp = Vector2f(-rb.y, rb.x);

        Vector2f angular_linear_velocity_a = ra_perp * body_a->getAngularVelocity();
        Vector2f angular_linear_velocity_b = rb_perp * body_b->getAngularVelocity();

        Vector2f v_diff = (body_b->getLinearVelocity()  + angular_linear_velocity_b) - (body_a->getLinearVelocity() + angular_linear_velocity_a);

        float contact_dot_vel = Math2D::dot(v_diff, normal);
        
        // only apply impulse if the object are moving apart
        if (contact_dot_vel > 0.f)
            continue;
        
        float angular_den_a = Math2D::dot(ra, normal) * Math2D::dot(ra, normal) * body_a->inv_inertia;
        float angular_den_b = Math2D::dot(rb, normal) * Math2D::dot(rb, normal) * body_b->inv_inertia;
        
        float j = -(1 + e) * contact_dot_vel;
        j /= body_a->inverse_mass + body_b->inverse_mass + angular_den_a + angular_den_b;
        j /= (float)contact_count;

        Vector2f impulse = normal * j;
        this->impulse_list_[i] = impulse;
    }

    // we do it in a separate for loop to gurantee that the impulse application does not affect the impulse calculationo
    for (int i = 0; i < contact_count; ++i)
    {
        Vector2f impulse = this->impulse_list_[i];
        Vector2f ra = this->ra_list_[i];
        Vector2f rb = this->rb_list_[i];

        body_a->setLinearVelocity(body_a->getLinearVelocity() - impulse * body_a->inv_inertia);
        body_a->setAngularVelocity(body_a->getAngularVelocity() - Math2D::cross(ra, impulse * body_a->inv_inertia));
        body_b->setLinearVelocity(body_b->getLinearVelocity() + impulse * body_b->inverse_mass);
        body_b->setAngularVelocity(body_b->getAngularVelocity() + Math2D::cross(rb, impulse * body_b->inv_inertia));
    }
}

void World2D::resolveCollisionWithRotationAndFriction(const CollisionManifold& collision)
{
    Rigidbody* body_a = collision.body_a;
    Rigidbody* body_b = collision.body_b;
    Vector2f normal = collision.normal;
    Vector2f contact_one = collision.contact_one;
    Vector2f contact_two = collision.contact_two;
    int contact_count = collision.contact_count;

    float e = std::min(body_a->restitution, body_b->restitution);

    float sf = (body_a->static_friction + body_b->static_friction) * 0.5f;
    float df = (body_a->dynamic_friction + body_b->dynamic_friction) * 0.5f;

    this->contact_list_[0] = contact_one;
    this->contact_list_[1] = contact_two;

    for (int i = 0; i < contact_count; ++i)
    {
        this->impulse_list_[i] = Vector2f::Zero();
        this->friction_impulse_list_[i] = Vector2f::Zero();
        this->j_list_[i] = 0;
        this->ra_list_[i] = Vector2f::Zero();
        this->rb_list_[i] = Vector2f::Zero();
    }

    for (int i = 0; i < contact_count; ++i)
    {
        Vector2f ra = this->contact_list_[i] - body_a->getPosition();
        Vector2f rb = this->contact_list_[i] - body_b->getPosition();

        this->ra_list_[i] = ra;
        this->rb_list_[i] = rb;
        
        Vector2f ra_perp = Vector2f(-ra.y, ra.x);
        Vector2f rb_perp = Vector2f(-rb.y, rb.x);

        Vector2f angular_linear_velocity_a = ra_perp * body_a->getAngularVelocity();
        Vector2f angular_linear_velocity_b = rb_perp * body_b->getAngularVelocity();

        Vector2f v_diff = (body_b->getLinearVelocity()  + angular_linear_velocity_b) - (body_a->getLinearVelocity() + angular_linear_velocity_a);

        float contact_dot_vel = Math2D::dot(v_diff, normal);
        
        // only apply impulse if the object are moving apart
        if (contact_dot_vel > 0.f)
            continue;
        
        float angular_den_a = Math2D::dot(ra, normal) * Math2D::dot(ra, normal) * body_a->inv_inertia;
        float angular_den_b = Math2D::dot(rb, normal) * Math2D::dot(rb, normal) * body_b->inv_inertia;
        
        float j = -(1 + e) * contact_dot_vel;
        j /= body_a->inverse_mass + body_b->inverse_mass + angular_den_a + angular_den_b;
        j /= (float)contact_count;

        j_list_[i] = j;

        Vector2f impulse = normal * j;
        this->impulse_list_[i] = impulse;
    }

    // we do it in a separate for loop to gurantee that the impulse application does not affect the impulse calculationo
    for (int i = 0; i < contact_count; ++i)
    {
        Vector2f impulse = this->impulse_list_[i];
        Vector2f ra = this->ra_list_[i];
        Vector2f rb = this->rb_list_[i];

        body_a->setLinearVelocity(body_a->getLinearVelocity() - impulse * body_a->inv_inertia);
        body_a->setAngularVelocity(body_a->getAngularVelocity() - Math2D::cross(ra, impulse * body_a->inv_inertia));
        body_b->setLinearVelocity(body_b->getLinearVelocity() + impulse * body_b->inverse_mass);
        body_b->setAngularVelocity(body_b->getAngularVelocity() + Math2D::cross(rb, impulse * body_b->inv_inertia));
    }

    // impulse for frictions
    for (int i = 0; i < contact_count; ++i)
    {
        Vector2f ra = this->contact_list_[i] - body_a->getPosition();
        Vector2f rb = this->contact_list_[i] - body_b->getPosition();

        this->ra_list_[i] = ra;
        this->rb_list_[i] = rb;
        
        Vector2f ra_perp = Vector2f(-ra.y, ra.x);
        Vector2f rb_perp = Vector2f(-rb.y, rb.x);

        Vector2f angular_linear_velocity_a = ra_perp * body_a->getAngularVelocity();
        Vector2f angular_linear_velocity_b = rb_perp * body_b->getAngularVelocity();

        Vector2f v_diff = (body_b->getLinearVelocity()  + angular_linear_velocity_b) - (body_a->getLinearVelocity() + angular_linear_velocity_a);

        Vector2f tangent = v_diff - normal * Math2D::dot(v_diff, normal);
        
        if (Math2D::nearlyEqual(tangent, Vector2f::Zero()))
            continue;
        else
            tangent = Math2D::normalize(tangent);


        float angular_den_a = Math2D::dot(ra, tangent) * Math2D::dot(ra, tangent) * body_a->inv_inertia;
        float angular_den_b = Math2D::dot(rb, tangent) * Math2D::dot(rb, tangent) * body_b->inv_inertia;
        
        float jt = -Math2D::dot(v_diff, tangent);
        jt /= body_a->inverse_mass + body_b->inverse_mass + angular_den_a + angular_den_b;
        jt /= (float)contact_count;

        // coloumb's law
        Vector2f friction_impulse;
        float j = this->j_list_[i];
        if (std::abs(jt) <= j * sf)
        {
            friction_impulse = normal * jt;
        } else
        {
            friction_impulse = -tangent * j * df;
        }

        this->friction_impulse_list_[i] = friction_impulse;
    }

    // we do it in a separate for loop to gurantee that the impulse application does not affect the impulse calculationo
    for (int i = 0; i < contact_count; ++i)
    {
        Vector2f friction_impulse = this->friction_impulse_list_[i];
        Vector2f ra = this->ra_list_[i];
        Vector2f rb = this->rb_list_[i];

        body_a->setLinearVelocity(body_a->getLinearVelocity() - friction_impulse * body_a->inv_inertia);
        body_a->setAngularVelocity(body_a->getAngularVelocity() - Math2D::cross(ra, friction_impulse * body_a->inv_inertia));
        body_b->setLinearVelocity(body_b->getLinearVelocity() + friction_impulse * body_b->inverse_mass);
        body_b->setAngularVelocity(body_b->getAngularVelocity() + Math2D::cross(rb, friction_impulse * body_b->inv_inertia));
    }
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
    std::vector<std::string>::iterator it = std::find(this->list_of_object_names.begin(), this->list_of_object_names.end(), name);
    if (it != this->list_of_object_names.end())
    {
        body = this->rigidbodies_[std::distance(this->list_of_object_names.begin(), it)];
        return true;
    }
    
    return false;
}

int World2D::getBodyCount() const
{
    return this->rigidbodies_.size();
}

void World2D::separateBodies(Rigidbody*& body_a, Rigidbody*& body_b, Vector2f& mtv)
{
    if (body_a->is_static)
    {
        body_b->move(mtv);
    } else if (body_b->is_static)
    {
        body_a->move(-mtv);
    } else
    {
        body_a->move(-mtv / 2.f);
        body_b->move(mtv / 2.f);
    }
}

void World2D::update(const sf::Time& dt, int substeps)
{
    substeps = Math2D::clip(substeps, this->min_substeps, this->max_substeps);
    this->contact_points.clear();
    this->contacts_.clear();
    
    for (int step = 0; step < substeps; ++step)
    {
        this->contact_pairs.clear();
        this->movementStep(dt, substeps); 
        this->broadPhase();
        this->narrowPhase(step, substeps);
    }
    
    // reset forces
    for (int i = 0; i < this->rigidbodies_.size(); ++i)
    {
        this->rigidbodies_[i]->setForce(Vector2f::Zero());
    }
}

void World2D::broadPhase()
{
    for (int i = 0; i < this->rigidbodies_.size() - 1; ++i)
    {
        Rigidbody*& body_a = this->rigidbodies_[i];
        AABB body_a_aabb = body_a->getAABB();

        for (int j = i + 1; j < this->rigidbodies_.size(); ++j)
        {
            Rigidbody*& body_b = this->rigidbodies_[j];
            AABB body_b_aabb = body_b->getAABB();

            if (body_a->is_static && body_b->is_static)
                continue;
            
            // AABB collision checking
            if (!Collision2D::intersectAABBs(body_a_aabb, body_b_aabb))
                continue;

            this->contact_pairs.push_back(Tuple(i, j));
        }
    }
}

void World2D::narrowPhase(const int current_step, const int substeps)
{
    Vector2f normal = Vector2f::Zero();
    float depth = 0;

    for (int i = 0; i < this->contact_pairs.size(); ++i)
    {
        Tuple indices = this->contact_pairs[i];
        Rigidbody* body_a = this->rigidbodies_[indices.item1];
        Rigidbody* body_b = this->rigidbodies_[indices.item2];

        // SAT collision checking
        if (Collision2D::collide(body_a, body_b, normal, depth))
        {
            Vector2f mtv = normal * depth;

            this->separateBodies(body_a, body_b, mtv);                

            Vector2f contact_one = Vector2f::Zero();
            Vector2f contact_two = Vector2f::Zero();
            int contact_count = 0;

            Collision2D::findContactPoint(body_a, body_b, contact_one, contact_two, contact_count);

            CollisionManifold collision = CollisionManifold(body_a, body_b, normal, depth, contact_one, contact_two, contact_count);
            // this->resolveCollisionBasic(collision);
            // this->resolveCollisionWithRotation(collision);
            this->resolveCollisionWithRotationAndFriction(collision);
            this->contacts_.push_back(collision);
        }
    }

    if (this->render_collision_points)
    {
        if (current_step == substeps - 1)
        {
            for (int i = 0; i < this->contacts_.size(); ++i)
            {
                CollisionManifold contact = this->contacts_[i];
                if (!(std::find<std::vector<Vector2f>::iterator, Vector2f>(this->contact_points.begin(), this->contact_points.end(), contact.contact_one) != this->contact_points.end()))
                    this->contact_points.push_back(contact.contact_one);

                if (contact.contact_count > 1)
                {
                    if (!(std::find<std::vector<Vector2f>::iterator, Vector2f>(this->contact_points.begin(), this->contact_points.end(), contact.contact_two) != this->contact_points.end()))
                        this->contact_points.push_back(contact.contact_two);
                }
            }
        }
    }
}

void World2D::movementStep(const sf::Time& dt, const int substeps)
{
    for (int i = 0; i < this->rigidbodies_.size(); ++i)
    {
        if (!this->rigidbodies_[i]->is_static)
            this->rigidbodies_[i]->update(dt, this->gravity_, substeps);
    }
}