#ifndef TWO_D_VECTOR_HPP
#define TWO_D_VECTOR_HPP

#include <functional>
#include <string>
#include "transform2d.hpp"

namespace Physics2D
{
    class Transform2D;
    struct Vector2f
    { 
        // constructor
        Vector2f();
        Vector2f(float x, float y);

        // variables
        float x;
        float y;
        static Vector2f Zero();

        // operations
        Vector2f operator+(const Vector2f& a) const;
        Vector2f operator-(const Vector2f& a) const;
        Vector2f operator-();
        Vector2f operator*(const float scalar) const;
        Vector2f operator/(const float scalar) const;
        Vector2f operator*=(const float scalar);
        Vector2f operator+=(const Vector2f& a);
        Vector2f operator-=(const Vector2f& a);
        bool operator==(const Vector2f& a);
        std::size_t getHashCode();
        std::string toString();
        static Vector2f transform(Vector2f& a, Transform2D& transform);
    };
}

namespace std
{
    template <>
    struct hash<Physics2D::Vector2f>
    {
        std::size_t operator()(const Physics2D::Vector2f& a)
        {
            std::size_t h1 = std::hash<float>{}(a.x);
            std::size_t h2 = std::hash<float>{}(a.y);
            return h1 ^(h2 << 1); // bitwise XOR and bit-shift for mixing
        }
    };
}

#endif