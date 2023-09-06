#ifndef AABB_HPP
#define AABB_HPP

#include "vector2d.hpp"

namespace Physics2D
{
    struct AABB
    {
        Vector2f min;
        Vector2f max;

        AABB();
        AABB(const Vector2f& min, const Vector2f& max);
        AABB(float min_x, float min_y, float max_x, float max_y);
    };
    
}

#endif