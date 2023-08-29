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

bool World::checkCollisionBetween(const std::string& name1, const std::string& name2, sf::Time dt)
{
    PhysicsObject& obj1 = (*(*this->objects_.find(name1)).second);
    PhysicsObject& obj2 = (*(*this->objects_.find(name2)).second);

    float smallest_penetration_distance = INFINITY;
    std::map<float, sf::Vector2f> pdistances_and_normal;
    sf::Vector2f normal_of_collision = sf::Vector2f(0.f, 0.f);

    std::vector<sf::Vector2f> corners_obj1;
    corners_obj1.push_back(obj1.getCornerPosition("tl"));
    corners_obj1.push_back(obj1.getCornerPosition("tr"));
    corners_obj1.push_back(obj1.getCornerPosition("bl"));
    corners_obj1.push_back(obj1.getCornerPosition("br"));

    std::vector<sf::Vector2f> normals = obj1.getNormals();

    std::vector<sf::Vector2f> corners_obj2;
    corners_obj2.push_back(obj2.getCornerPosition("tl"));
    corners_obj2.push_back(obj2.getCornerPosition("tr"));
    corners_obj2.push_back(obj2.getCornerPosition("bl"));
    corners_obj2.push_back(obj2.getCornerPosition("br"));

    std::vector<sf::Vector2f> normal2 = obj2.getNormals();
    normals.push_back(normal2[0]);
    normals.push_back(normal2[1]);

    for (auto n : normals)
    {
        std::array<float, 4> projections;
        int counter = 0;
        for (auto c : corners_obj1)
        {
            float dot = n.x * c.x + n.y * c.y;
            projections[counter] = dot;
            ++counter;
        }
        
        float obj1_max = *std::max_element(projections.begin(), projections.end());
        float obj1_min = *std::min_element(projections.begin(), projections.end());

        counter = 0;
        for (auto c : corners_obj2)
        {
            float dot = n.x * c.x + n.y * c.y;
            projections[counter] = dot;
            ++counter;
        }

        float obj2_max = *std::max_element(projections.begin(), projections.end());
        float obj2_min = *std::min_element(projections.begin(), projections.end());

        bool overlap = (obj1_max >= obj2_min && obj1_max <= obj2_max) || (obj1_min >= obj2_min && obj1_min <= obj2_max); 
        if (overlap)
        {
            if (obj1_max >= obj2_min && obj1_max <= obj2_max)
                pdistances_and_normal.emplace(obj1_max - obj2_min, n);
            else if (obj1_min >= obj2_min && obj1_min <= obj2_max)
                pdistances_and_normal.emplace(obj2_max - obj1_min, n);
        }
        else
        {
            return false;
        }
    }

    // find minimal penetration distance
    for (auto pair : pdistances_and_normal)
    {
        if (pair.first < smallest_penetration_distance)
        {
            smallest_penetration_distance = pair.first;
            normal_of_collision = pair.second;
        }
    }

    std::cout << "COLLISION between " << name1 << " and " << name2 << std::endl;
    if (name1 == "tester")
    {
        obj1.getBody().setFillColor(sf::Color::Green);
    }

    if (name1 == "goal")
    {
        obj1.setRandomPosition();
        obj2.increaseScoreBy(1);
    }
    else if (name2 == "goal")
    {
        obj2.setRandomPosition();
        obj1.increaseScoreBy(1);
    }

    if (name1 == "enemy" || name2 == "enemy" || name2.find("wall") != std::string::npos)
    {
        if (name1 != "tester")
        {
            this->reset();
            this->game_over = true;
        }
    }

    return true;
}

void World::wallCollision()
{
    for (auto obj : this->objects_)
    {
        if (obj.second->is_moving_)
        {
            // right wall
            if (obj.second->getBody().getPosition().x + obj.second->getSize().x/2 > this->window_->getWindowSize().x)
            {
                obj.second->getBody().setPosition(sf::Vector2f(this->window_->getWindowSize().x - obj.second->getSize().x/2, obj.second->getBody().getPosition().y));
                obj.second->setPosition(sf::Vector2f(0, obj.second->getPosition().y));
                obj.second->setVelocity(sf::Vector2f(0.f, obj.second->getVelocity().y));
            }
            // left wall
            if (obj.second->getBody().getPosition().x - obj.second->getSize().x/2 < 0)
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
}

void World::update(const sf::Time& dt)
{
    // check for walls
    wallCollision();
    checkCollisionBetween("goal", "player", dt);
    // checkCollisionBetween("player", "enemy", dt);

    bool tester_collided = false;
    this->getObject("tester").getBody().setFillColor(sf::Color::Blue);
    for (auto name : this->list_of_object_names_)
    {
        if (name.find("wall") != std::string::npos)
        {
            checkCollisionBetween("player", name, dt);
            if (!tester_collided)
                tester_collided = checkCollisionBetween("tester", name, dt);
        }
    }

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