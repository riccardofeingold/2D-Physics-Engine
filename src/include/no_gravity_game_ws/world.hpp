#ifndef WORLD_HPP
#define WORLD_HPP

#include "rigidbody.hpp"
#include "vector2dconverter.hpp"
#include "collision2d.hpp"
#include "window.hpp"
#include "ray.hpp"
#include "collisionManifold.hpp"
#include <iostream>

namespace Physics2D
{
    // forward declaration 
    class Ray;
    class Rigidbody;
    class CollisionManifold;
    
    template <typename T1, typename T2>
    struct Tuple
    {
        T1 item1;
        T2 item2;

        Tuple(T1 item1, T2 item2)
        {
            this->item1 = item1;
            this->item2 = item2;
        }
    };

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
        void resolveCollision(const CollisionManifold& collision);

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
        std::vector<Vector2f> contact_points;
        std::vector<Tuple<int, int>> contact_pairs;
        bool render_collision_points = false;

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
        std::vector<CollisionManifold> contacts_;

        // private methods
        void separateBodies(Rigidbody*& body_a, Rigidbody*& body_b, Vector2f& mtv);
        void broadPhase();
        void narrowPhase(const int current_step, const int substeps);
        void movementStep(const sf::Time& dt, const int substeps);
    };
}

#endif