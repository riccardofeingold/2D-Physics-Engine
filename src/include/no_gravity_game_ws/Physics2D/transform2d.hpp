#ifndef TRANSFORM_2D_HPP
#define TRANSFORM_2D_HPP
#include "math2d.hpp"

namespace Physics2D
{
    class Vector2f;
    struct Transform2D
    {
        Transform2D(Vector2f& position, float radians);
        Transform2D(float x, float y, float radians);
        
        static Transform2D Zero();
        float position_x; 
        float position_y;
        float sin;
        float cos;
    }; 
} // namespace Physics2D


#endif