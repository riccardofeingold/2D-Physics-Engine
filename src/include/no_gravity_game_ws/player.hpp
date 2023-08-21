#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "physics.hpp"
#include "colors.hpp"

#define ANGLE_DIFF 1

class Player : public PhysicsObject
{
    public:
    Player(float x, float y, sf::Vector2f size, float restitution = 1.f);
    ~Player();

    // methods
    void move() override;
    void reset() override;
    void setRandomPosition() override;
    void move(const sf::Vector2f& position); // used with keys
};

#endif