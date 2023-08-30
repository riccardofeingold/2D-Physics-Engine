#ifndef COLLISION_2D_HPP
#define COLLISION_2D_HPP

#include "math2d.hpp"

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
        static bool circleCollisionDetection(Vector2f& center_a, float radius_a, Vector2f& center_b, float radius_b, Vector2f& normal, float& depth);

        private:

    }; 
} // namespace Physics2D


#endif