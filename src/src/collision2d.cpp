#include "../include/no_gravity_game_ws/collision2d.hpp"

using namespace Physics2D;

bool Collision2D::circleCollisionDetection(const Vector2f& center_a, const float radius_a, const Vector2f& center_b, const float radius_b, Vector2f& normal, float& depth)
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
    normal = Vector2f::Zero();
    depth = INFINITY;

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

        float penetration_depth = std::min(max_b - min_a, max_a - min_b);

        if (penetration_depth < depth)
        {
            depth = penetration_depth;
            normal = axis;
        }

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
        

        float penetration_depth = std::min(max_b - min_a, max_a - min_b);

        if (penetration_depth < depth)
        {
            depth = penetration_depth;
            normal = axis;
        }
    }

    Vector2f center_to_center = Collision2D::findArithmeticMean(vertices_b) - Collision2D::findArithmeticMean(vertices_a);
    if (Math2D::dot(normal, center_to_center) < 0)
        normal = -normal;

    return true;
}

bool Collision2D::circlePolygonCollisionDetection(const Vector2f& circle_center, const float circle_radius, const std::vector<Vector2f>& vertices, Vector2f& normal, float& depth)
{
    float min_a, max_a, min_b, max_b;
    normal = Vector2f::Zero();
    depth = INFINITY;
    Vector2f axis = Vector2f::Zero();
    float penetration_depth = 0.f;

    for (int i = 0; i < vertices.size(); ++i)
    {
        Vector2f vertex_a = vertices[i];
        Vector2f vertex_b = vertices[(i + 1) % vertices.size()];

        Vector2f edge_ab = vertex_b - vertex_a;
        axis = Vector2f(-edge_ab.y, edge_ab.x);
        axis = Math2D::normalize(axis);

        Collision2D::projectVertices(vertices, axis, min_a, max_a);
        Collision2D::projectCircle(circle_center, circle_radius, axis, min_b, max_b);
        
        if (min_a >= max_b || min_b >= max_a)
            return false;

        penetration_depth = std::min(max_b - min_a, max_a - min_b);

        if (penetration_depth < depth)
        {
            depth = penetration_depth;
            normal = axis;
        }
    }

    int cp_index = Collision2D::closestPoint(circle_center, vertices);
    axis = vertices[cp_index] - circle_center;
    axis = Math2D::normalize(axis);

    Collision2D::projectVertices(vertices, axis, min_a, max_a);
    Collision2D::projectCircle(circle_center, circle_radius, axis, min_b, max_b);
    
    if (min_a >= max_b || min_b >= max_a)
        return false;

    penetration_depth = std::min(max_b - min_a, max_a - min_b);

    if (penetration_depth < depth)
    {
        depth = penetration_depth;
        normal = axis;
    }

    Vector2f center_to_center = Collision2D::findArithmeticMean(vertices) - circle_center;
    if (Math2D::dot(normal, center_to_center) < 0)
        normal = -normal;

    return true; 
}

int Collision2D::closestPoint(const Vector2f& point, const std::vector<Vector2f>& vertices)
{
    int index = -1;
    float min = INFINITY;
    
    for (int i = 0; i < vertices.size(); ++i)
    {
        float distance = Math2D::distance(point, vertices[i]);
        if (distance < min)
        {
            min = distance;
            index = i;
        }
    }

    return index;
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

void Collision2D::projectCircle(const Vector2f& center, const float radius, const Vector2f& axis, float& min, float& max)
{
    Vector2f normalized_axis = Math2D::normalize(axis);
    Vector2f scaled_normal = normalized_axis * radius;
    Vector2f point1 = center + scaled_normal;
    Vector2f point2 = center - scaled_normal;
    
    min = Math2D::dot(axis, point1);
    max = Math2D::dot(axis, point2);

    if (max < min)
        Collision2D::swap(min, max);
}

void Collision2D::swap(float& value1, float& value2)
{
    float temp = value1;
    value1 = value2;
    value2 = temp;
}

Vector2f Collision2D::findArithmeticMean(const std::vector<Vector2f>& vertices)
{
    float sum_x = 0.f;
    float sum_y = 0.f;

    for (int i = 0; i < vertices.size(); ++i)
    {
        Vector2f v = vertices[i];
        sum_x += v.x;
        sum_y += v.y;
    }

    return Vector2f(sum_x/(float)vertices.size(), sum_y/(float)vertices.size());
}