#include "../include/no_gravity_game_ws/physics.hpp"

PhysicsObject::PhysicsObject()
    : gravity_(9.81),
      mass_(1),
      position_(sf::Vector2f(0.f, 0.f)),
      velocity_(sf::Vector2f(0.f, 0.f)),
      acceleration_(sf::Vector2f(0.f, 0.f))
{
}

PhysicsObject::PhysicsObject(float gravity, float mass) 
    : gravity_(gravity), 
      mass_(mass),
      position_(sf::Vector2f(0.f, 0.f)), 
      velocity_(sf::Vector2f(0.f, 0.f)), 
      acceleration_(sf::Vector2f(0.f, 0.f))
{
}

PhysicsObject::~PhysicsObject() = default;

// getters 
const float& PhysicsObject::getMass() const { return mass_; }

const sf::Vector2f& PhysicsObject::getPosition() const { return position_; }

const sf::Vector2f& PhysicsObject::getVelocity() const { return velocity_; }

const sf::Vector2f& PhysicsObject::getAcceleration() const { return acceleration_; }

const float& PhysicsObject::getOrientation() const { return orientation_; }

const float& PhysicsObject::getAngularVelocity() const { return angular_velocity_; }

const float& PhysicsObject::getAngularAcceleration() const { return angular_acceleration_; }

const int& PhysicsObject::getId() const { return this->id_; }

const sf::Vector2f& PhysicsObject::getSize() const { return this->size_; }

sf::RectangleShape& PhysicsObject::getBody() { return this->body_; }

int PhysicsObject::getScore() const { return this->score_; }

// @brief returns a list of all normals needed for SAT
std::vector<sf::Vector2f> PhysicsObject::getNormals()
{
    std::vector<sf::Vector2f> normals;
    
    sf::Vector2f normal = sf::Vector2f(this->getCornerPosition("tr").y - this->getCornerPosition("rl").y, this->getCornerPosition("tl").x - this->getCornerPosition("tr").x);
    normals.push_back(normal);

    normal = sf::Vector2f(this->getCornerPosition("bl").y - this->getCornerPosition("tl").y, this->getCornerPosition("tl").x - this->getCornerPosition("bl").y);
    normals.push_back(normal);

    return normals;
}

// @brief possible arguments: tl = top left, bl = bottom left, tr = top right, br = bottom right
sf::Vector2f PhysicsObject::getCornerPosition(std::string corner_name)
{
    float cos_theta = std::cos((float)this->getBody().getRotation()*M_PI/180);
    float sin_theta = std::sin((float)this->getBody().getRotation()*M_PI/180);

    if ("tl" == corner_name)
    {
        float local_x = cos_theta * (-this->getSize().x/2) - sin_theta * (-this->getSize().y/2);
        float local_y = sin_theta * (-this->getSize().x/2) + cos_theta * (-this->getSize().y/2);

        float x = this->getBody().getPosition().x + local_x;
        float y = this->getBody().getPosition().y + local_y;

        return sf::Vector2f(x, y);
    }
    else if ("bl" == corner_name)
    {
        float local_x = cos_theta * (-this->getSize().x/2) - sin_theta * (this->getSize().y/2);
        float local_y = sin_theta * (-this->getSize().x/2) + cos_theta * (this->getSize().y/2);
        
        float x = this->getBody().getPosition().x + local_x;
        float y = this->getBody().getPosition().y + local_y;

        return sf::Vector2f(x, y);
    }
    else if ("tr" == corner_name)
    {
        float local_x = cos_theta * (this->getSize().x/2) - sin_theta * (-this->getSize().y/2);
        float local_y = sin_theta * (this->getSize().x/2) + cos_theta * (-this->getSize().y/2);

        float x = this->getBody().getPosition().x + local_x;
        float y = this->getBody().getPosition().y + local_y;

        return sf::Vector2f(x, y);
    }
    else if ("br" == corner_name)
    {
        float local_x = cos_theta * (this->getSize().x/2) - sin_theta * (this->getSize().y/2);
        float local_y = sin_theta * (this->getSize().x/2) + cos_theta * (this->getSize().y/2);

        float x = this->getBody().getPosition().x + local_x;
        float y = this->getBody().getPosition().y + local_y;

        return sf::Vector2f(x, y);
    }
    else
        return sf::Vector2f(0, 0);
}

// setters
void PhysicsObject::setAcceleration(const sf::Vector2f& acceleration) { this->acceleration_ = acceleration; }

void PhysicsObject::setVelocity(const sf::Vector2f& velocity) { this->velocity_ = velocity; }

void PhysicsObject::setPosition(const sf::Vector2f& position) { this->position_ = position; }

void PhysicsObject::setOrientation(float angle) { this->orientation_ = angle; }

void PhysicsObject::setAngularVelocity(float angular_velocity) { this->angular_velocity_ = angular_velocity; }

void PhysicsObject::setAngularAcceleration(float angular_acceleration) { this->angular_acceleration_ = angular_acceleration; }

void PhysicsObject::setMass(float mass) { this->mass_ = mass; }

void PhysicsObject::setId(int id) { this->id_ = id; }

void PhysicsObject::setWindow(Window& window) { this->window_ = &window; }

void PhysicsObject::setRandomPosition()
{
    sf::Vector2f random_position = sf::Vector2f((float)std::rand()/RAND_MAX*this->window_->getWindowSize().x, (float)std::rand()/RAND_MAX*this->window_->getWindowSize().y);
    this->body_.setPosition(random_position);
}

void PhysicsObject::setScoreToZero() { this->score_ = 0; }

void PhysicsObject::increaseScoreBy(int num)
{
    this->score_ += num;
}
// Physics
// @brief applies a force at the center of mass
void PhysicsObject::applyForce(const sf::Vector2f& force)
{
    acceleration_ += force / mass_;
}

void PhysicsObject::applyImpulse(const sf::Vector2f& impulse)
{
    velocity_ += impulse / mass_;
}

void PhysicsObject::applyTorque(const float& torque)
{
    angular_acceleration_ += torque / moment_of_inertia_.getMomentOfInertia();
}

void PhysicsObject::update(sf::Time delta_time)
{
    // apply gravity
    acceleration_.y += gravity_;
    
    // apply air drag translational
    sf::Vector2f air_drag = air_drag_.force(velocity_);
    acceleration_.x += air_drag.x / mass_;
    acceleration_.y += air_drag.y / mass_;    

    // apply air drag rotational
    float air_drag_angular = air_drag_.torque(angular_velocity_);
    angular_acceleration_ += air_drag_angular / moment_of_inertia_.getMomentOfInertia();

    // calculate velocity and position
    velocity_ += acceleration_ * delta_time.asSeconds();
    position_ += velocity_ * delta_time.asSeconds();
    acceleration_ = sf::Vector2f(0.f, 0.f);

    // calculate rotation
    angular_velocity_ += angular_acceleration_ * delta_time.asSeconds();
    orientation_ += angular_velocity_ * delta_time.asSeconds();
    angular_acceleration_ = 0.f;
}

void PhysicsObject::applyAirDrag(float Cd, float A, float density)
{
    air_drag_.drag_coefficient = Cd;
    air_drag_.cross_sectional_area = A;
    air_drag_.density = density;
}