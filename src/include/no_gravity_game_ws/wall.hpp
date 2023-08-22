#ifndef WALL_HPP
#define WALL_HPP

#include "physics.hpp"
#include "colors.hpp"

class Wall : public PhysicsObject
{
    public:
    Wall(const sf::Vector2f& position, const sf::Vector2f& size, float rotation);
    ~Wall();

    void move() override;
    void reset() override;
    void update(sf::Time delta_time) override;
};

#endif