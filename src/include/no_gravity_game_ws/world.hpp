#pragma once
#include <SFML/Graphics.hpp>
#include "physics.hpp"
#include <vector>

class World
{
    public:
    World();
    ~World();

    void addObject(PhysicsObject* obj);
    void removeObject(PhysicsObject* obj);
    void setup();
    void update(const sf::Time& dt);
    
    private:
    std::vector<PhysicsObject*> objects_;

};