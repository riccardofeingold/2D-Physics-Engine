#pragma once
#include "physics.hpp"
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
