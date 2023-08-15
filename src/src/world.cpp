#include "../include/no_gravity_game_ws/world.hpp"

World::World()
{
}

World::~World() = default;

void World::addObject(PhysicsObject* obj)
{
    this->objects_.push_back(obj);
    obj->setId(this->objects_.size() - 1);
}

void World::removeObject(PhysicsObject* obj)
{
    if (!obj) return;
    else
    {
        std::vector<PhysicsObject*>::iterator iter = std::find(this->objects_.begin(), this->objects_.end(), obj);
        this->objects_.erase(iter);
    }
}

void World::update(const sf::Time& dt)
{
    for (PhysicsObject* obj : objects_)
    {
        obj->update(dt);
        obj->move();
    }
}

// getters
PhysicsObject& World::getObject(const int id)
{
    return (*this->objects_.at(id));
}

int World::getNumberOfObjects() const { return this->objects_.size(); }