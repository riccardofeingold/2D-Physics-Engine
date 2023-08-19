#include "../include/no_gravity_game_ws/world.hpp"

World::World()
{
}

World::~World() = default;

void World::reset()
{
    for (auto obj : this->objects_)
    {
        obj.second->reset();
    }
}

void World::addObject(const std::string& name, PhysicsObject* obj)
{
    this->objects_.emplace(name, obj);
    this->list_of_object_names_.push_back(name);
}

void World::removeObject(const std::string& name)
{
    std::map<std::string, PhysicsObject*>::iterator iter = this->objects_.find(name);
    this->objects_.erase(iter);
}

void World::checkCollisionBetween(const std::string& name1, const std::string& name2)
{
    PhysicsObject& obj1 = (*(*this->objects_.find(name1)).second);
    PhysicsObject& obj2 = (*(*this->objects_.find(name2)).second);

    // axis aligned boundary box
    if 
    (
        obj1.getBody().getPosition().x < obj2.getBody().getPosition().x + obj2.getSize().x &&
        obj1.getBody().getPosition().x + obj1.getSize().x > obj2.getBody().getPosition().x &&
        obj1.getBody().getPosition().y < obj2.getBody().getPosition().y + obj2.getSize().y &&
        obj1.getBody().getPosition().y + obj1.getSize().y > obj2.getBody().getPosition().y
    )
    {
        if (name1 == "goal")
            obj1.setRandomPosition();
        else if (name2 == "goal")
            obj2.setRandomPosition();
    }
}

void World::wallCollision()
{
    for (auto obj : this->objects_)
    {
        // right wall
        if (obj.second->getBody().getPosition().x + obj.second->getSize().x/2 > this->window_->getWindowSize().x)
        {
            obj.second->getBody().setPosition(sf::Vector2f(this->window_->getWindowSize().x - obj.second->getSize().x/2, obj.second->getBody().getPosition().y));
            obj.second->setPosition(sf::Vector2f(0, obj.second->getPosition().y));
            obj.second->setVelocity(sf::Vector2f(0.f, obj.second->getVelocity().y));
        }
        // left wall
        if (obj.second->getBody().getPosition().x + obj.second->getSize().x/2 < 0)
        {
            obj.second->getBody().setPosition(sf::Vector2f(obj.second->getSize().x/2, obj.second->getBody().getPosition().y));
            obj.second->setPosition(sf::Vector2f(0, obj.second->getPosition().y));
            obj.second->setVelocity(sf::Vector2f(0.f, obj.second->getVelocity().y));
        }
        // bottom wall
        if (obj.second->getBody().getPosition().y + obj.second->getSize().y/2 > this->window_->getWindowSize().y)
        {
            obj.second->getBody().setPosition(sf::Vector2f(obj.second->getBody().getPosition().x, this->window_->getWindowSize().y - obj.second->getSize().y/2));
            obj.second->setPosition(sf::Vector2f(obj.second->getPosition().x, 0));
            obj.second->setVelocity(sf::Vector2f(obj.second->getVelocity().x, 0.f));
        }
        // top wall
        if (obj.second->getBody().getPosition().y - obj.second->getSize().y/2 < 0)
        {
            obj.second->getBody().setPosition(sf::Vector2f(obj.second->getBody().getPosition().x, obj.second->getSize().y/2));
            obj.second->setPosition(sf::Vector2f(obj.second->getPosition().x, 0));
            obj.second->setVelocity(sf::Vector2f(obj.second->getVelocity().x, 0.f));
        }
    }
}

void World::update(const sf::Time& dt)
{
    // check for walls
    wallCollision();
    checkCollisionBetween("goal", "player");
    for (auto obj : this->objects_)
    {
        obj.second->update(dt);
        obj.second->move();
    }
}

// getters
PhysicsObject& World::getObject(const std::string& name)
{
    return (*(*this->objects_.find(name)).second);
}

int World::getNumberOfObjects() const { return this->objects_.size(); }