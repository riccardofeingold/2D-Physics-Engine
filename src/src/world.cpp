#include "../include/no_gravity_game_ws/world.hpp"

World::World()
{
}

World::~World() = default;

void World::reset()
{
    for (PhysicsObject* obj : this->objects_)
    {
        obj->reset();
    }
}

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

void World::wallCollision()
{
    for (PhysicsObject* obj : this->objects_)
    {
        // right wall
        if (obj->getBody().getPosition().x + obj->getSize().x/2 > this->window_->getWindowSize().x)
        {
            obj->getBody().setPosition(sf::Vector2f(this->window_->getWindowSize().x - obj->getSize().x/2, obj->getBody().getPosition().y));
            obj->setPosition(sf::Vector2f(0, obj->getPosition().y));
            obj->setVelocity(sf::Vector2f(0.f, obj->getVelocity().y));
        }
        // left wall
        if (obj->getBody().getPosition().x + obj->getSize().x/2 < 0)
        {
            obj->getBody().setPosition(sf::Vector2f(obj->getSize().x/2, obj->getBody().getPosition().y));
            obj->setPosition(sf::Vector2f(0, obj->getPosition().y));
            obj->setVelocity(sf::Vector2f(0.f, obj->getVelocity().y));
        }
        // bottom wall
        if (obj->getBody().getPosition().y + obj->getSize().y/2 > this->window_->getWindowSize().y)
        {
            obj->getBody().setPosition(sf::Vector2f(obj->getBody().getPosition().x, this->window_->getWindowSize().y - obj->getSize().y/2));
            obj->setPosition(sf::Vector2f(obj->getPosition().x, 0));
            obj->setVelocity(sf::Vector2f(obj->getVelocity().x, 0.f));
        }
        // top wall
        if (obj->getBody().getPosition().y - obj->getSize().y/2 < 0)
        {
            obj->getBody().setPosition(sf::Vector2f(obj->getBody().getPosition().x, obj->getSize().y/2));
            obj->setPosition(sf::Vector2f(obj->getPosition().x, 0));
            obj->setVelocity(sf::Vector2f(obj->getVelocity().x, 0.f));
        }
    }
}

void World::update(const sf::Time& dt)
{
    // check for walls
    wallCollision();

    for (PhysicsObject* obj : this->objects_)
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