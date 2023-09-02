#include "../include/no_gravity_game_ws/aabb.hpp"

using namespace Physics2D;

AABB::AABB()
{
    this->min = Vector2f::Zero();
    this->max = Vector2f::Zero();
}

AABB::AABB(const Vector2f& min, const Vector2f& max)
{
    this->min = min;
    this->max = max;
}

AABB::AABB(float min_x, float min_y, float max_x, float max_y)
{
    this->min = Vector2f(min_x, min_y);
    this->max = Vector2f(max_x, max_y);
}