#include "../include/no_gravity_game_ws/collision2d.hpp"

using namespace Physics2D;

bool Collision2D::circleCollisionDetection(Vector2f& center_a, float radius_a, Vector2f& center_b, float radius_b, Vector2f& normal, float& depth)
{
    normal = Vector2f::Zero();
    depth = 0.f;

    float diff_distance = Math2D::distance(center_a, center_b);
    float sum_radii = radius_a + radius_b;
    
    if (diff_distance >= sum_radii)
    {
        return false;
    }
    
    normal = center_b - center_a;
    normal = Math2D::normalize(normal);
    depth = sum_radii - diff_distance;
    
    return true;        
}