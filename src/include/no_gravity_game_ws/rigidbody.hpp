#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "world.hpp"
#include "math2d.hpp"

namespace Physics2D
{
    enum ShapeType
    {
        Circle = 0,
        Box = 1,
        Triangle = 2,
        Polygon = 3
    };
    
    class Rigidbody
    {
        public:
        Rigidbody();
        Rigidbody(const Vector2f& position, float density, float mass, float restitution, float area, bool isStatic, float radius, float width, float height, ShapeType shape_type);
        ~Rigidbody();

        // TODO: add a color variable
        Vector2f position_;
        Vector2f linear_velocity_;
        float rotation_;
        float angular_velocity_;

        float density_;
        float mass_;
        float restitution_;
        float area_;

        bool is_static_;
     
        float radius_;
        float width_;
        float height_;

        sf::Color color_;

        ShapeType shape_type_;

        // methods
        /// @brief  create a circle rigidbody. Important the body pointer needs to be delted properly after usage, since it's dynamically allocated.
        /// @param width 
        /// @param height 
        /// @param position 
        /// @param density 
        /// @param is_static 
        /// @param restitution 
        /// @param body 
        /// @param error_message 
        /// @return true if successfully initialized otherwise false
        static bool createCircleBody(float radius, Vector2f position, float density, bool is_static, float restitution, Rigidbody*& body, std::string& error_message);
        
        /// @brief create a box rigidbody.
        /// @param width 
        /// @param height 
        /// @param position 
        /// @param density 
        /// @param is_static 
        /// @param restitution 
        /// @param body 
        /// @param error_message 
        /// @return true if successfully initialized otherwise false
        static bool createBoxBody(float width, float height, Vector2f position, float density, bool is_static, float restitution, Rigidbody*& body, std::string& error_message);

        /// @brief moving the rigidbody by the vector move
        /// @param move - vector
        void move(const Vector2f& move);

        /// @brief move the rigidbody to a absolute position
        /// @param move - vector
        void moveTo(const Vector2f& move);

        /// @brief random color
        /// @return sf::Color 
        static sf::Color randomColor(); 
        private:
    };
}

#endif