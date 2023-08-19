#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "momentOfInertia.hpp"

class DragForce
{
    public:
    float drag_coefficient = 0.f;
    float cross_sectional_area = 0.f;
    float density = 0.f;

    // methods
    float constantMultiplier()
    {
        return 0.5 * drag_coefficient * cross_sectional_area * density;
    }

    sf::Vector2f force(const sf::Vector2f& velocity)
    {
        sf::Vector2f force;
        if (velocity.x >= 0)
            force.x = -constantMultiplier() * velocity.x * velocity.x;
        else if (velocity.x < 0)
            force.x = constantMultiplier() * velocity.x * velocity.x;
        
        if (velocity.y >= 0)
            force.y = -constantMultiplier() * velocity.y * velocity.y;
        else if (velocity.y < 0)
            force.y = constantMultiplier() * velocity.y * velocity.y;

        return force;
    }

    float torque(const float angular_velocity)
    {
        if (angular_velocity >= 0)
            return -constantMultiplier() * angular_velocity * angular_velocity;
        if (angular_velocity < 0)
            return constantMultiplier() * angular_velocity * angular_velocity;
        return 0;
    }
};

struct Material
{
    sf::Color color = sf::Color(0, 0, 0, 255);
    float restitution = 0.f;
    float density = 0.f;
    float friction = 0.f;
};

class PhysicsObject
{
    public:
    PhysicsObject();
    PhysicsObject(float gravity, float mass);
    ~PhysicsObject();

    // Methods
    virtual void applyForce(const sf::Vector2f& force);
    void applyTorque(const float& torque);
    void applyImpulse(const sf::Vector2f& impulse);
    void update(sf::Time delta_time);
    void applyAirDrag(float Cd, float A, float density);

    // pure virtual methods
    virtual void move() = 0;
    virtual void reset() = 0;

    // getters
    virtual const sf::Vector2f& getPosition() const;
    virtual const sf::Vector2f& getVelocity() const;
    virtual const sf::Vector2f& getAcceleration() const;
    virtual const float& getOrientation() const;
    virtual const float& getAngularVelocity() const;
    virtual const float& getAngularAcceleration() const;
    virtual const float& getMass() const;
    virtual const int& getId() const;
    virtual sf::RectangleShape& getBody();
    virtual const sf::Vector2f& getSize() const;

    // setters
    virtual void setAcceleration(const sf::Vector2f& acceleration);
    virtual void setVelocity(const sf::Vector2f& velocity);
    virtual void setPosition(const sf::Vector2f& position);
    virtual void setOrientation(float angle);
    virtual void setAngularVelocity(float angular_velocity);
    virtual void setAngularAcceleration(float angular_acceleration);
    virtual void setMass(float mass);
    virtual void setId(int id);

    protected:
    sf::Vector2f initial_position_;
    sf::Vector2f position_;
    sf::Vector2f velocity_;
    sf::Vector2f acceleration_;
    float orientation_;
    float angular_velocity_;
    float angular_acceleration_;

    sf::RectangleShape body_;
    sf::Vector2f size_;
    Material material_;
    MomentOfInertia moment_of_inertia_;

    private:
    // private methods

    // private variables
    int id_;
    DragForce air_drag_;
    float gravity_;
    float mass_;
};