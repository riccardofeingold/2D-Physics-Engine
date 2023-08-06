#pragma once
#include <SFML/Graphics.hpp>

class Player
{
    public:
    Player(float x, float y, float size, float restitution = 1.f);
    ~Player();

    // methods
    void move();
    void jump();
    void shoot();
    
    // public variables
    float size;
    float restitution;
    sf::CircleShape body;
    sf::Vector2f velocity;
    
    private:
    bool on_ground_;
};