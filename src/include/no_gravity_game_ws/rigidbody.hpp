#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "world.hpp"
#include "math2d.hpp"
#include <SFML/Graphics.hpp>

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

        float density; // in kg/m^3
        float mass; // in kg
        float restitution;
        float area; // m^2

        bool is_static;
     
        float radius; // in meters
        float width; // in meters
        float height; // in meters

        sf::Color color; // rgba

        ShapeType shape_type;
        
        std::vector<int> triangle_indices;

        // getters
        Vector2f getForce() const;
        Vector2f getPosition() const;
        Vector2f getLinearVelocity() const;
        float getRotation() const;
        float getAngularVelocity() const;

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
        
        /// @brief rotate object
        /// @param amount how many degrees do you want it to rotate
        void rotate(const float amount);
        
        /// @brief rotate to a fixed orientation
        /// @param rotation 
        void rotateTo(const float rotation);

        /// @brief random color
        /// @return sf::Color 
        static sf::Color randomColor(); 
        
        /// @brief transforms the vertices if a transform update is required
        /// @return returns a list of all transformed vertices of a shape
        const std::vector<Vector2f>& getTransformedVertices();

        /// @brief update the rigidbodies physics
        /// @param dt delta time
        void update(const sf::Time& dt);

        /// @brief applies a force onto the rigidbody
        /// @param force 
        void applyForce(const Vector2f& force);

        private:
        Vector2f force_; // N
        Vector2f position_; // in meters = 1 pixel
        Vector2f linear_velocity_; // in m/s
        float rotation_; // in radians
        float angular_velocity_; // radians/s

        std::vector<Vector2f> vertices_;
        std::vector<Vector2f> transformed_vertices_;
        bool transform_update_required_;

        /// @brief create the vertices of a box; is used to tranform those points accordingly
        /// @param width 
        /// @param height 
        /// @return list of vertices
        static std::vector<Vector2f> createBoxVertices(float width, float height);

        /// @brief get the indices of the vertices of the two triangles that make up the box
        /// @return list of traingle vertex indices
        static std::vector<int> createBoxTriangleIndices();
    };
}

#endif