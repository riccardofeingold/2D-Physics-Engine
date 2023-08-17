#pragma once
#include <SFML/Graphics.hpp>
#include "physics.hpp"

class Player : public PhysicsObject
{
    public:
    Player(float x, float y, sf::Vector2f size, float restitution = 1.f);
    ~Player();

    // methods
    void move();
    void move(const sf::Vector2f& position); // used with keys
    void shoot();
    void reset();
};