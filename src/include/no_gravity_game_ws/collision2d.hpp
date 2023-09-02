#ifndef COLLISION_2D_HPP
#define COLLISION_2D_HPP

#include "math2d.hpp"
#include <iostream>
#include <map>

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
        static bool circleCollisionDetection(const Vector2f& center_a, const float radius_a, const Vector2f& center_b, const float radius_b, Vector2f& normal, float& depth);
        
        /// @brief checking for collision between polygons
        /// @param vertices_a 
        /// @param vertices_b 
        /// @return true if collided otherwise false 
        static bool polygonCollisionDetection(const std::vector<Vector2f>& vertices_a, const std::vector<Vector2f>& vertices_b, Vector2f& normal, float& depth);

        /// @brief checking for collision between polygons
        /// @param vertices_a 
        /// @param center_a 
        /// @param vertices_b 
        /// @param center_b 
        /// @param normal 
        /// @param depth 
        /// @return true if collided otherwise false  
        static bool polygonCollisionDetection(const std::vector<Vector2f>& vertices_a, const Vector2f& center_a, const std::vector<Vector2f>& vertices_b, const Vector2f& center_b, Vector2f& normal, float& depth);

        /// @brief check for circle polygon collision
        /// @param circle_center 
        /// @param circle_radius 
        /// @param vertices 
        /// @param normal 
        /// @param depth 
        /// @return true if collision detected otherwise false
        static bool circlePolygonCollisionDetection(const Vector2f& circle_center, const float circle_radius, const std::vector<Vector2f>& vertices, Vector2f& normal, float& depth);

        /// @brief check for circle collision
        /// @param circle_center 
        /// @param circle_radius
        /// @param polygon_center 
        /// @param vertices 
        /// @param normal 
        /// @param depth 
        /// @return true if collision detected otherwise false
        static bool circlePolygonCollisionDetection(const Vector2f& circle_center, const float circle_radius, const Vector2f& polygon_center, const std::vector<Vector2f>& vertices, Vector2f& normal, float& depth);

        /// @brief project the vertices of a polygon onto the axis
        /// @param vertices 
        /// @param axis 
        /// @return min and max value of the projected vertices
        static void projectVertices(const std::vector<Vector2f>& vertices, const Vector2f& axis, float& min, float& max);

        /// @brief calculates the geometric center of the polygon
        /// @param vertices 
        /// @return returns the location of the center of the object
        static Vector2f findArithmeticMean(const std::vector<Vector2f>& vertices);

        /// @brief find the index of the closest vertex to the point
        /// @param point 
        /// @param vertices 
        /// @return index of closest vertex
        static int closestPoint(const Vector2f& point, const std::vector<Vector2f>& vertices);
        private:

        /// @brief project the "vertices" of a circle
        /// @param center 
        /// @param radius 
        /// @param axis 
        /// @return min and max value of the projected vertices 
        static void projectCircle(const Vector2f& center, const float radius, const Vector2f& axis, float& min, float& max);

        /// @brief swap the two values
        /// @param value1 
        /// @param value2 
        static void swap(float& value1, float& value2);
    }; 
} // namespace Physics2D


#endif