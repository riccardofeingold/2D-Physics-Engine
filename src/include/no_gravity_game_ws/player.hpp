#pragma once
#include <SFML/Graphics.hpp>
#include "physics.hpp"

class Player : public PhysicsObject
{
    public:
    Player(float x, float y, float size, float restitution = 1.f);
    ~Player();

    // methods
    void move();
    void move(const sf::Vector2f& position); // used with keys
    void shoot();
    
    // setters
    void setVelocity(const sf::Vector2f& velocity);

    // getters
    const sf::Vector2f& getVelocity() const;
    
    // public variable
    bool on_ground;
    float size;
    float restitution;

    private:
    sf::Vector2f velocity_;
};