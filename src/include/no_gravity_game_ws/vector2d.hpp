#ifndef TWO_D_VECTOR_HPP
#define TWO_D_VECTOR_HPP

#include <functional>
#include <string>

namespace Physics2D
{
    struct Vector2f
    { 
        // constructor
        Vector2f(float x, float y);

        // variables
        float x;
        float y;
        static Vector2f Zero();

        // operations
        Vector2f operator+(Vector2f& a);
        Vector2f operator-(Vector2f& a);
        Vector2f operator-();
        Vector2f operator*(float scalar);
        Vector2f operator/(float scalar);
        bool operator==(Vector2f& a);
        std::size_t getHashCode();
        std::string toString();
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