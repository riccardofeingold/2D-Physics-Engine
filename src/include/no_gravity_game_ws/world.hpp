#pragma once
#include <SFML/Graphics.hpp>
#include "physics.hpp"
#include "window.hpp"
#include <iostream>
#include <vector>

class World
{
    public:
    World();
    World(float gravity);
    ~World();

    // methods
    void addObject(PhysicsObject* obj);
    void removeObject(PhysicsObject* obj);

    void setup();
    void update(const sf::Time& dt);
    void reset();

    // collisions
    void wallCollision();

    // getters
    PhysicsObject& getObject(const int id);
    int getNumberOfObjects() const;

    // public variables
    Window* window_;

    private:
    std::vector<PhysicsObject*> objects_;
};