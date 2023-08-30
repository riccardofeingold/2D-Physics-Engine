#ifndef MATH_2D_HPP
#define MATH_2D_HPP

#include "vector2d.hpp"

namespace Physics2D
{
    class Vector2f;
    class Math2D
    {
        public:
        Math2D();

        // other math stuff
        /// @brief clipping a value between min and max
        static float clip(float value, float min, float max);
        
        /// @brief convert to degree from radian
        /// @param value 
        /// @return degrees
        static float convertToDegree(float radian);
        
        /// @brief convert to radian from degree
        /// @param degree 
        /// @return radians
        static float convertToRadian(float degree);
        
        // Vector math
        /// @brief calculate the length of a vector
        static float norm(Vector2f& a);
        /// @brief calculate the distance between two vectors
        static float distance(Vector2f& a, Vector2f& b);
        /// @brief normalize vector
        static Vector2f normalize(Vector2f& a);
        /// @brief dot product
        static float dot(Vector2f& a);
        /// @brief cross product
        static float cross(Vector2f& a, Vector2f& b);
    }; 
} // namespace Physics2D


#endif