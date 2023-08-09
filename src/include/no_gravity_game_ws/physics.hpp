#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

struct DragForce
{
    float drag_coefficient = 0.f;
    float cross_sectional_area = 0.f;
    float density = 0.f;
    float force = 0.f;
};

class PhysicsObject
{
    public:
    PhysicsObject(float gravity, float mass, float moment_of_inertia);
    ~PhysicsObject();

    // Methods
    void apply_force(const sf::Vector2f& force);
    void apply_torque(const float& torque);
    void apply_impulse(const sf::Vector2f& impulse);
    void update(sf::Time delta_time);
    void addAirResitance();

    // getters
    const sf::Vector2f& get_position() const;
    const sf::Vector2f& get_velocity() const;
    const sf::Vector2f& get_acceleration() const;
    const float& get_orientation() const;
    const float& get_angular_velocity() const;
    const float& get_angular_acceleration() const;
    const float& get_mass() const;
    const float& get_moment_of_inertia() const;

    private:
    // private methods
    void calculateDragForce();

    // private variables
    DragForce air_drag_;
    float gravity_;
    float mass_;
    float moment_of_inertia_;
    sf::Vector2f position_;
    sf::Vector2f velocity_;
    sf::Vector2f acceleration_;
    float orientation_;
    float angular_velocity_;
    float angular_acceleration_;
};