#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "colors.hpp"
#include "physics.hpp"
#include <SFML/Graphics.hpp>

class Enemy : public PhysicsObject
{
    public:
    Enemy(sf::Vector2f size, float restitution);
    ~Enemy();

    void move() override;
    void reset() override;
    void update(sf::Time dt) override;
    
    void setTarget(const sf::Vector2f* target);
    void setSpeed(float speed);
    float getSpeed() const;

    private:
    const sf::Vector2f* target_position_;
    float SPEED;
};

#endif