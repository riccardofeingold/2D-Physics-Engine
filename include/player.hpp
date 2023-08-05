#pragma once
#include <SFML/Graphics.hpp>

class Player
{
    public:
    Player(float x, float y, float size);
    ~Player();

    // methods
    void move();
    void jump();
    void shoot();
    
    // public variables
    sf::CircleShape shape_;
    private:
    bool on_ground_;
};