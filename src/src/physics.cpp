#include "../include/no_gravity_game_ws/physics.hpp"

PhysicsObject::PhysicsObject()
    : gravity_(9.81),
      mass_(1),
      moment_of_inertia_(1),
      position_(sf::Vector2f(0.f, 0.f)),
      velocity_(sf::Vector2f(0.f, 0.f)),
      acceleration_(sf::Vector2f(0.f, 0.f))
{
}

PhysicsObject::PhysicsObject(float gravity, float mass, float moment_of_inertia) 
    : gravity_(gravity), 
      mass_(mass),
      moment_of_inertia_(moment_of_inertia),
      position_(sf::Vector2f(0.f, 0.f)), 
      velocity_(sf::Vector2f(0.f, 0.f)), 
      acceleration_(sf::Vector2f(0.f, 0.f))
{
}

PhysicsObject::~PhysicsObject() = default;

// getters 
const float& PhysicsObject::get_mass() const { return mass_; }

const float& PhysicsObject::get_moment_of_inertia() const { return moment_of_inertia_; }

const sf::Vector2f& PhysicsObject::get_position() const { return position_; }

const sf::Vector2f& PhysicsObject::get_velocity() const { return velocity_; }

const sf::Vector2f& PhysicsObject::get_acceleration() const { return acceleration_; }

const float& PhysicsObject::get_orientation() const { return orientation_; }

const float& PhysicsObject::get_angular_velocity() const { return angular_velocity_; }

const float& PhysicsObject::get_angular_acceleration() const { return angular_acceleration_; }

// Physics
void PhysicsObject::apply_force(const sf::Vector2f& force)
{
    acceleration_ += force / mass_;
}

void PhysicsObject::apply_impulse(const sf::Vector2f& impulse)
{
    velocity_ += impulse / mass_;
}

void PhysicsObject::apply_torque(const float& torque)
{
    angular_acceleration_ += torque / moment_of_inertia_;
}

void PhysicsObject::update(sf::Time delta_time)
{
    // apply gravity
    acceleration_.y += gravity_;
    
    // apply air drag
    sf::Vector2f air_drag = air_drag_.force(velocity_);
    acceleration_.x += air_drag.x / mass_;
    acceleration_.y += air_drag.y / mass_;

    // calculate velocity and position
    velocity_ += acceleration_ * delta_time.asSeconds();
    position_ += velocity_ * delta_time.asSeconds();
    acceleration_ = sf::Vector2f(0.f, 0.f);
}

void PhysicsObject::applyAirDrag(float Cd, float A, float density)
{
    air_drag_.drag_coefficient = Cd;
    air_drag_.cross_sectional_area = A;
    air_drag_.density = density;
}