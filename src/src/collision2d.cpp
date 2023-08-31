#include "../include/no_gravity_game_ws/collision2d.hpp"

using namespace Physics2D;

bool Collision2D::circleCollisionDetection(Vector2f& center_a, const float radius_a, Vector2f& center_b, const float radius_b, Vector2f& normal, float& depth)
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

bool Collision2D::polygonCollisionDetection(const std::vector<Vector2f>& vertices_a, const std::vector<Vector2f>& vertices_b, Vector2f& normal, float& depth)
{
    float min_a, max_a, min_b, max_b;
    
    for (int i = 0; i < vertices_a.size(); ++i)
    {
        Vector2f vertex_a = vertices_a[i];
        Vector2f vertex_b = vertices_a[(i + 1) % vertices_a.size()];

        Vector2f edge_ab = vertex_b - vertex_a;
        Vector2f axis = Vector2f(-edge_ab.y, edge_ab.x);
        axis = Math2D::normalize(axis);

        Collision2D::projectVertices(vertices_a, axis, min_a, max_a);
        Collision2D::projectVertices(vertices_b, axis, min_b, max_b);

        if (min_a >= max_b || min_b >= max_a)
            return false;
    }

    for (int i = 0; i < vertices_b.size(); ++i)
    {
        Vector2f vertex_a = vertices_b[i];
        Vector2f vertex_b = vertices_b[(i + 1) % vertices_b.size()];

        Vector2f edge_ab = vertex_b - vertex_a;
        Vector2f axis = Vector2f(-edge_ab.y, edge_ab.x);
        axis = Math2D::normalize(axis);

        Collision2D::projectVertices(vertices_a, axis, min_a, max_a);
        Collision2D::projectVertices(vertices_b, axis, min_b, max_b);

        if (min_a >= max_b || min_b >= max_a)
            return false;
    }

    return true;
}

void Collision2D::projectVertices(const std::vector<Vector2f>& vertices, const Vector2f& axis, float& min, float& max)
{
    min = INFINITY;
    max = -INFINITY;

    for (int i = 0; i < vertices.size(); ++i)
    {
        float projection = Math2D::dot(vertices[i], axis);
        if (projection < min)
            min = projection;
        if (projection > max)
            max = projection;
    }
}