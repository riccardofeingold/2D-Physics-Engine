#pragma once
#include <SFML/Graphics.hpp>
#include "physics.hpp"
#include "window.hpp"
#include <iostream>
#include <map>

class World
{
    public:
    World();
    World(float gravity);
    ~World();

    // methods
    void addObject(const std::string& name, PhysicsObject* obj);
    void removeObject(const std::string& name);

    void setup();
    void update(const sf::Time& dt);
    void reset();

    // collisions
    void wallCollision();
    void checkCollisionBetween(const std::string& name1, const std::string& name2);

    // getters
    PhysicsObject& getObject(const std::string& name);
    int getNumberOfObjects() const;

    // public variables
    Window* window_;
    std::vector<std::string> list_of_object_names_;
    bool game_over = false;

    private:
    std::map<std::string, PhysicsObject*> objects_;
};