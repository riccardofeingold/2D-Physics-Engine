#ifndef WORLD_HPP
#define WORLD_HPP

#include "physics.hpp"
#include "window.hpp"
#include "ray.hpp"
#include <iostream>
#include <map>

// forward declaration 
class Ray;
class PhysicsObject;

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
    bool checkCollisionBetween(const std::string& name1, const std::string& name2, sf::Time dt);

    // getters
    PhysicsObject& getObject(const std::string& name);
    int getNumberOfObjects() const;

    // public variables
    Window* window_;
    std::vector<std::string> list_of_object_names_;
    bool game_over = false;

    // constant values
    static constexpr float min_body_size = 0.01f * 0.01f;
    static constexpr float max_body_size = 100.f * 100.f;
    static constexpr float min_density = 0.25f;
    static constexpr float max_density = 22.f;

    // friend classes
    friend class Ray;
    
    private:
    std::map<std::string, PhysicsObject*> objects_;
};

#endif