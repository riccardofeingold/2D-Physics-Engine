#pragma once
#include "physics.hpp"
#include <SFML/Graphics.hpp>
#include <cstdlib>

class Goal : public PhysicsObject
{
    public:
    Goal();
    ~Goal();

    // setters
    void setRandomPosition();

    private:
    bool player_reached_it_;
};
