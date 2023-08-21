#ifndef RAY_HPP
#define RAY_HPP

#include "world.hpp"
#include <cmath>

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
    float calculateMagnitude(sf::Vector2f& end_point);

    // getters
    const sf::RectangleShape& getLineShape() const;
    const float getDistance() const;
    const float getAngle() const;

    private:
    sf::RectangleShape line_;
    sf::Vector2f direction_;
    const sf::Vector2f* start_;
    const float angle_; // degrees
    float distance_;
    sf::Vector2f point_of_contact_;
};

#endif