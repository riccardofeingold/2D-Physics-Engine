#include "../../include/no_gravity_game_ws/Physics2D/collisionManifold.hpp"

using namespace Physics2D;

CollisionManifold::CollisionManifold(Rigidbody*& body_a, Rigidbody*& body_b, Vector2f& normal, float& depth, Vector2f& contact_one, Vector2f& contact_two, int& contact_count)
{
    this->body_a = body_a;
    this->body_b = body_b;
    this->normal = normal;
    this->depth = depth;
    this->contact_one = contact_one;
    this->contact_two = contact_two;
    this->contact_count = contact_count;
}

bool CollisionManifold::operator==(const CollisionManifold& object)
{
    return (this->body_a == object.body_a &&
            this->body_b == object.body_b &&
            this->normal == object.normal &&
            this->depth == object.depth &&
            this->contact_one == object.contact_one &&
            this->contact_two == object.contact_two &&
            this->contact_count == object.contact_count);
}