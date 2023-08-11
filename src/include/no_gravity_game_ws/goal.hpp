#pragma once

#include <SFML/Graphics.hpp>
#include <cstdlib>

class Goal
{
    public:
    Goal(float height, float width, float screen_height, float screen_width);
    ~Goal();

    // setters
    void setRandomPosition();

    // public variables
    sf::RectangleShape body_;

    private:
    bool player_reached_it_;
    float width_;
    float height_;
    float screen_width_;
    float screen_height_;
};
