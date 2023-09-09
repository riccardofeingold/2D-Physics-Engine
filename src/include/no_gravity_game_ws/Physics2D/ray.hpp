#ifndef RAY_HPP
#define RAY_HPP

#include "world.hpp"
#include <cmath>


#define THICKNESS 0.1

namespace Physics2D
{
    // forward declarations
    class World2D;
    class Rigidbody;

    class Ray
    {
        public:
        Ray(Rigidbody*& body, float angle);
        ~Ray();

        void castRay(World2D& w);
        void draw();
        float calculateMagnitude(sf::Vector2f& end_point);

        // getters
        const sf::RectangleShape& getLineShape() const;
        const float getDistance() const;
        const float getAngle() const;
        float getDistance();
        sf::Vector2f getPointOfContact();

        static constexpr float RANGE = 5.f; // 5 m

        private:
        void checkLeftSide(const Vector2f& player, const Vector2f& tl, const Vector2f& tr, const Vector2f& br, const Vector2f& bl, std::vector<sf::Vector2f>& points);
        void checkRightSide(const Vector2f& player, const Vector2f& tl, const Vector2f& tr, const Vector2f& br, const Vector2f& bl, std::vector<sf::Vector2f>& points);
        void checkTopSide(const Vector2f& player, const Vector2f& tl, const Vector2f& tr, const Vector2f& br, const Vector2f& bl, std::vector<sf::Vector2f>& points);
        void checkBottomSide(const Vector2f& player, const Vector2f& tl, const Vector2f& tr, const Vector2f& br, const Vector2f& bl, std::vector<sf::Vector2f>& points);

        bool lineCircleIntersection(const Vector2f& player, Rigidbody* const & body, Vector2f& point);

        sf::RectangleShape line_;
        sf::Vector2f direction_;
        const float angle_; // degrees
        float distance_;
        sf::Vector2f point_of_contact_;
        Rigidbody* body_;
    };
}

#endif