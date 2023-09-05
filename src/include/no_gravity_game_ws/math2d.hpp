#ifndef MATH_2D_HPP
#define MATH_2D_HPP

#include "vector2d.hpp"

namespace Physics2D
{
    class Vector2f;

    #define EPSILON 0.0005f;

    class Math2D
    {
        public:
        Math2D();

        // other math stuff
        /// @brief clipping a value between min and max
        static float clip(float value, float min, float max);
        static int clip(int value, int min, int max);
        
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
        static float norm(const Vector2f& a);
        /// @brief calculate the squared length of a vector
        static float norm_squared(const Vector2f& a);
        /// @brief calculate the distance between two vectors
        static float distance(const Vector2f& a, const Vector2f& b);
        /// @brief calculate the distance squared between two vectors
        static float distance_squared(const Vector2f& a, const Vector2f& b);
        /// @brief normalize vector
        static Vector2f normalize(const Vector2f& a);
        /// @brief dot product
        static float dot(const Vector2f& a, const Vector2f& b);
        /// @brief cross product
        static float cross(const Vector2f& a, const Vector2f& b);
        /// @brief returns true if x and y are almost the same
        static bool nearlyEqual(const float x, const float y);
        /// @brief returns true if vector one is almost equal to the second vector
        static bool nearlyEqual(const Vector2f& v1, const Vector2f& v2);
    }; 
} // namespace Physics2D


#endif