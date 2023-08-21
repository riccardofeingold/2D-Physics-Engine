#ifndef GOAL_HPP
#define GOAL_HPP

#include "physics.hpp"
#include "colors.hpp"
#include <SFML/Graphics.hpp>
#include <cstdlib>

class Goal : public PhysicsObject
{
    public:
    Goal(sf::Vector2f size, float restitution);
    ~Goal();

    void reset() override;
    void move() override;

    // setters
    void setRandomPosition() override;
};

#endif