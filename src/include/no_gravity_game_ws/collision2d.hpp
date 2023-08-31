#ifndef COLLISION_2D_HPP
#define COLLISION_2D_HPP

#include "math2d.hpp"
#include <iostream>

namespace Physics2D
{
    class Collision2D
    {
        public:
        Collision2D() = default;

        // methods
        /// @brief checking for circle collisions
        /// @param center_a
        /// @param radius_a
        /// @param center_b
        /// @param radius_b
        /// @return @param depth @param normal and bool indicating if a collision has been detected or not 
        static bool circleCollisionDetection(Vector2f& center_a, const float radius_a, Vector2f& center_b, const float radius_b, Vector2f& normal, float& depth);
        
        /// @brief checking for collision between polygons
        /// @param vertices_a 
        /// @param vertices_b 
        /// @return true if collided otherwise false 
        static bool polygonCollisionDetection(const std::vector<Vector2f>& vertices_a, const std::vector<Vector2f>& vertices_b, Vector2f& normal, float& depth);
        
        /// @brief project the vertices of a polygon onto the axis
        /// @param vertices 
        /// @param axis 
        /// @return min and max value of the projected vertices
        static void projectVertices(const std::vector<Vector2f>& vertices, const Vector2f& axis, float& min, float& max);
        private:

    }; 
} // namespace Physics2D


#endif