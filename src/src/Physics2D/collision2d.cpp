#include "../../include/no_gravity_game_ws/Physics2D/collision2d.hpp"

using namespace Physics2D;

bool Collision2D::collide(Rigidbody*& body_a, Rigidbody*& body_b, Vector2f& normal, float& depth)
{
    normal = Vector2f::Zero();
    depth = 0.f;

    ShapeType shape_type_a = body_a->shape_type;
    ShapeType shape_type_b = body_b->shape_type;

    if (shape_type_a == ShapeType::Box)
    {
        if (shape_type_b == ShapeType::Box)
        {
            return Collision2D::polygonCollisionDetection(body_a->getTransformedVertices(), body_a->getPosition(), body_b->getTransformedVertices(), body_b->getPosition(), normal, depth);
        } else if (shape_type_b == ShapeType::Circle)
        {
            bool result = Collision2D::circlePolygonCollisionDetection(body_b->getPosition(), body_b->radius, body_a->getPosition(), body_a->getTransformedVertices(), normal, depth);
            normal = -normal;
            return result;
        }
    } else if (shape_type_a == ShapeType::Circle)
    {
        if (shape_type_b == ShapeType::Box)
        {
            return Collision2D::circlePolygonCollisionDetection(body_a->getPosition(), body_a->radius, body_b->getPosition(), body_b->getTransformedVertices(), normal, depth);   
        } else if (shape_type_b == ShapeType::Circle)
        {
            return Collision2D::circleCollisionDetection(body_a->getPosition(), body_a->radius, body_b->getPosition(), body_b->radius, normal, depth);   
        }
    }

    return false;
}

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

void Collision2D::findContactPoint(Rigidbody*& body_a, Rigidbody*& body_b, Vector2f& contact_one, Vector2f& contact_two, int& contact_count)
{
    contact_count = 0;
    contact_one = Vector2f::Zero();
    contact_two = Vector2f::Zero();

    ShapeType shape_type_a = body_a->shape_type;
    ShapeType shape_type_b = body_b->shape_type;

    if (shape_type_a == ShapeType::Box)
    {
        if (shape_type_b == ShapeType::Box)
        {
            Collision2D::findPolygonsContactPoint(body_a->getTransformedVertices(), body_b->getTransformedVertices(), contact_one, contact_two, contact_count);
        } else if (shape_type_b == ShapeType::Circle)
        {
            Collision2D::findCirclePolygonContactPoint(body_b->getPosition(), body_b->radius, body_a->getTransformedVertices(), body_a->getPosition(), contact_one);
            contact_count = 1;
        }
    } else if (shape_type_a == ShapeType::Circle)
    {
        if (shape_type_b == ShapeType::Box)
        {
            Collision2D::findCirclePolygonContactPoint(body_a->getPosition(), body_a->radius, body_b->getTransformedVertices(), body_b->getPosition(), contact_one);
            contact_count = 1;
        } else if (shape_type_b == ShapeType::Circle)
        {
            Collision2D::findCirclesContactPoint(body_a->getPosition(), body_a->radius, body_b->getPosition(), body_b->radius, contact_one);
            contact_count = 1;
        }
    }
}

void Collision2D::findCirclesContactPoint(const Vector2f& circle_center_a, const float radius_a, const Vector2f& circle_center_b, const float radius_b, Vector2f& contact_point)
{
    Vector2f v_diff = circle_center_b - circle_center_a;
    v_diff = Math2D::normalize(v_diff);

    contact_point = circle_center_a + v_diff * radius_a;
}

void Collision2D::findCirclePolygonContactPoint(const Vector2f& circle_center, const float radius, const std::vector<Vector2f>& vertices, const Vector2f& polygon_center, Vector2f& contact_point)
{
    float min_distance = INFINITY;
    contact_point = Vector2f::Zero();

    for (int i = 0; i < vertices.size(); ++i)
    {
        Vector2f va = vertices[i];
        Vector2f vb = vertices[(i + 1) % vertices.size()];

        float distance_squared;
        Vector2f cp;
        pointSegmentDistance(circle_center, va, vb, distance_squared, cp);

        if (distance_squared < min_distance)
        {
            min_distance = distance_squared;
            contact_point = cp;
        }    
    }
}

void Collision2D::findPolygonsContactPoint(const std::vector<Vector2f>& vertices_a, const std::vector<Vector2f>& vertices_b, Vector2f& contact_one, Vector2f& contact_two, int& contact_count)
{
    contact_one = Vector2f::Zero();
    contact_two = Vector2f::Zero();
    contact_count = 0;

    float min_distance_squared = INFINITY;

    for (int i = 0; i < vertices_a.size(); ++i)
    {
        Vector2f p = vertices_a[i];

        for (int j = 0; j < vertices_b.size(); ++j)
        {
            Vector2f va = vertices_b[j];
            Vector2f vb = vertices_b[(j + 1) % vertices_b.size()];

            float distance_squared;
            Vector2f cp = Vector2f::Zero();
            pointSegmentDistance(p, va, vb, distance_squared, cp);

            if (Math2D::nearlyEqual(distance_squared, min_distance_squared))
            {
                if (!Math2D::nearlyEqual(cp, contact_one))
                {
                    contact_two = cp;
                    contact_count = 2;
                }
            } else if (distance_squared < min_distance_squared)
            {
                min_distance_squared = distance_squared;
                contact_one = cp;
                contact_count = 1;
            }
        }
    }

    for (int i = 0; i < vertices_b.size(); ++i)
    {
        Vector2f p = vertices_b[i];

        for (int j = 0; j < vertices_a.size(); ++j)
        {
            Vector2f va = vertices_a[j];
            Vector2f vb = vertices_a[(j + 1) % vertices_a.size()];

            float distance_squared;
            Vector2f cp = Vector2f::Zero();
            pointSegmentDistance(p, va, vb, distance_squared, cp);

            if (Math2D::nearlyEqual(distance_squared, min_distance_squared))
            {
                if (!Math2D::nearlyEqual(cp, contact_one))
                {
                    contact_two = cp;
                    contact_count = 2;
                }
            } else if (distance_squared < min_distance_squared)
            {
                min_distance_squared = distance_squared;
                contact_one = cp;
                contact_count = 1;
            }
        }
    }
}

void Collision2D::pointSegmentDistance(const Vector2f& p, const Vector2f& point_begin, const Vector2f& point_end, float& distance_squared, Vector2f& contact_point)
{
    Vector2f ab = point_end - point_begin;
    Vector2f ap = p - point_begin;

    float proj = Math2D::dot(ap, ab);
    float ab_mag = Math2D::norm_squared(ab); // ab dot ab
    float ratio = proj / ab_mag;

    if (ratio <= 0.f)
        contact_point = point_begin;
    else if (ratio >= 1.f)
        contact_point = point_end;
    else
        contact_point = point_begin + ab * ratio;

    distance_squared = Math2D::distance_squared(p, contact_point);
}

bool Collision2D::polygonCollisionDetection(const std::vector<Vector2f>& vertices_a, const Vector2f& center_a, const std::vector<Vector2f>& vertices_b, const Vector2f& center_b, Vector2f& normal, float& depth)
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

    Vector2f center_to_center = center_b - center_a;
    if (Math2D::dot(normal, center_to_center) < 0)
        normal = -normal;

    return true; 
}

bool Collision2D::circlePolygonCollisionDetection(const Vector2f& circle_center, const float circle_radius, const Vector2f& polygon_center, const std::vector<Vector2f>& vertices, Vector2f& normal, float& depth)
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

    Vector2f center_to_center = polygon_center - circle_center;
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

bool Collision2D::intersectAABBs(const AABB& a, const AABB& b)
{
    if (a.max.x <= b.min.x ||
        a.min.x >= b.max.x ||
        a.max.y <= b.min.y ||
        a.min.y >= b.max.y)
        return false;
    return true;
}