#pragma once
#include <SFML/Graphics.hpp>

class Player
{
    public:
    Player(float x, float y, float size, float restitution = 1.f);
    ~Player();

    // methods
    void move(const sf::Vector2f& position);
    void shoot();
    
    // setters
    void setVelocity(const sf::Vector2f& velocity);

    // getters
    const sf::Vector2f& getVelocity() const;
    
    // public variable
    bool on_ground;
    float size;
    float restitution;
    sf::CircleShape body;

    private:
    sf::Vector2f velocity_;
};