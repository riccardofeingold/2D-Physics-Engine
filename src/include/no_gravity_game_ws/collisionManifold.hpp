#ifndef COLLISION_MANIFOLD_HPP
#define COLLISION_MANIFOLD_HPP

#include "rigidbody.hpp"

namespace Physics2D
{
    // forward declerations
    class Rigidbody;

    struct CollisionManifold
    {
        Rigidbody* body_a;
        Rigidbody* body_b;
        Vector2f normal;
        float depth;
        Vector2f contact_one;
        Vector2f contact_two;
        int contact_count;

        CollisionManifold(Rigidbody*& body_a, Rigidbody*& body_b, Vector2f& normal, float& depth, Vector2f& contact_one, Vector2f& contact_two, int& contact_count);
    };
} // namespace Physics2D


#endif