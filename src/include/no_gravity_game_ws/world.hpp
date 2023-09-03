#ifndef WORLD_HPP
#define WORLD_HPP

#include "rigidbody.hpp"
#include "vector2dconverter.hpp"
#include "collision2d.hpp"
#include "window.hpp"
#include "ray.hpp"
#include <iostream>

namespace Physics2D
{
    // forward declaration 
    class Ray;
    class Rigidbody;

    class World2D
    {
        public:
        World2D();
        World2D(float gravity);
        ~World2D();

        // methods
        void addObject(const std::string& name, Rigidbody* obj);
        bool removeObject(const std::string& name);
        bool collide(Rigidbody*& body_a, Rigidbody*& body_b, Vector2f& normal, float& depth);
        void resolveCollision(Rigidbody*& body_a, Rigidbody*& body_b, const Vector2f& normal, const float& depth);

        void setup();
        void update(const sf::Time& dt, int substeps);
        void reset();

        // getters
        bool getBody(int index, Rigidbody*& body);
        bool getBody(std::string name, Rigidbody*& body);
        int getBodyCount() const;

        // public variables
        Window* window_;
        std::vector<std::string> list_of_object_names_;
        bool game_over = false;

        // constant values
        static constexpr float min_body_size = 0.01f * 0.01f;
        static constexpr float max_body_size = 100.f * 100.f;
        static constexpr float min_density = 0.25f;
        static constexpr float max_density = 22.f;
        static constexpr int min_substeps = 1;
        static constexpr int max_substeps = 128;

        // friend classes
        friend class Ray;
        
        private:
        Vector2f gravity_;
        std::vector<Rigidbody*> rigidbodies_;
        std::vector<sf::Color> outline_color_;
    };
}

#endif