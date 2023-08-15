#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

struct DragForce
{
    float drag_coefficient = 0.f;
    float cross_sectional_area = 0.f;
    float density = 0.f;

    // methods
    float constant_multiplier()
    {
        return 0.5 * drag_coefficient * cross_sectional_area * density;
    }

    sf::Vector2f force(const sf::Vector2f& velocity)
    {
        sf::Vector2f force;
        if (velocity.x >= 0)
            force.x = -constant_multiplier() * velocity.x * velocity.x;
        else if (velocity.x < 0)
            force.x = constant_multiplier() * velocity.x * velocity.x;
        
        if (velocity.y >= 0)
            force.y = -constant_multiplier() * velocity.y * velocity.y;
        else if (velocity.y < 0)
            force.y = constant_multiplier() * velocity.y * velocity.y;

        return force;
    }
};

class PhysicsObject
{
    public:
    PhysicsObject();
    PhysicsObject(float gravity, float mass, float moment_of_inertia);
    ~PhysicsObject();

    // Methods
    void apply_force(const sf::Vector2f& force);
    void apply_torque(const float& torque);
    void apply_impulse(const sf::Vector2f& impulse);
    void update(sf::Time delta_time);
    void applyAirDrag(float Cd, float A, float density);

    // getters
    virtual const sf::Vector2f& get_position() const;
    virtual const sf::Vector2f& get_velocity() const;
    virtual const sf::Vector2f& get_acceleration() const;
    virtual const float& get_orientation() const;
    virtual const float& get_angular_velocity() const;
    virtual const float& get_angular_acceleration() const;
    virtual const float& get_mass() const;
    virtual const float& get_moment_of_inertia() const;

    protected:
    sf::Vector2f position_;
    sf::Vector2f velocity_;
    sf::Vector2f acceleration_;
    float orientation_;
    float angular_velocity_;
    float angular_acceleration_;

    private:
    // private methods

    // private variables
    DragForce air_drag_;
    float gravity_;
    float mass_;
    float moment_of_inertia_;
};