#ifndef RAY_HPP
#define RAY_HPP

#include "world.hpp"
#include <cmath>
#include <SFML/Graphics.hpp>

// forward declarations
class World;

#define THICKNESS 1
#define MAX_DISTANCE 200

class Ray
{
    public:
    Ray(const sf::Vector2f* start, float angle);
    ~Ray();

    void castRay(World& w);
    void draw();

    // public variable
    sf::RectangleShape line_;

    private:
    sf::Vector2f direction_;
    const sf::Vector2f* start_;
    const float angle_; // degrees
    sf::Vector2f point_of_contact_;
};

#endif