#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "world.hpp"
#include "math2d.hpp"
#include "aabb.hpp"
#include <SFML/Graphics.hpp>

namespace Physics2D
{
    // forward declarations
    class Ray;
    
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
        Rigidbody(float density, float inertia, float mass, float restitution, float area, bool isStatic, float radius, float width, float height, const float static_friction, const float dynamic_friction, std::vector<Vector2f>& vertices, ShapeType shape_type, bool apply_gravity = false, bool angle_fix = false);
        ~Rigidbody();
    
        ShapeType shape_type;
        float density; // in kg/m^3
        float mass; // in kg
        float inverse_mass;
        float restitution;
        float area; // m^2
        float inertia;
        float inv_inertia;
        bool is_static;
        float radius; // in meters
        float width; // in meters
        float height; // in meters
        float static_friction;
        float dynamic_friction;
        bool angle_fix = false;

        std::vector<Ray> rays;
        
        // getters
        Vector2f getForce() const;
        Vector2f getPosition() const;
        Vector2f getLinearVelocity() const;
        float getRotation() const;
        float getAngularVelocity() const;

        // setters
        void setLinearVelocity(const Vector2f& value);
        void setAngularVelocity(const float value);
        void setForce(const Vector2f& value);

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
        static bool createCircleBody(const float radius, const float density, const bool is_static, float restitution, const float static_friction, const float dynamic_friction, Rigidbody*& body, std::string& error_message, const bool apply_gravity = false, const bool angle_fix = false);
        
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
        static bool createBoxBody(const float width, const float height, const float density, const bool is_static, float restitution, const float static_friction, const float dynamic_friction, Rigidbody*& body, std::string& error_message, const bool apply_gravity = false, const bool angle_fix = false);

        /// @brief moving the rigidbody by the vector move
        /// @param move - vector
        void move(const Vector2f& move);

        /// @brief move the rigidbody to a absolute position
        /// @param move - vector
        void moveTo(const Vector2f& move);
        
        /// @brief rotate object
        /// @param amount how many radians do you want it to rotate
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
        void update(const sf::Time& dt, const Vector2f& gravity, int substeps);

        /// @brief applies a force onto the rigidbody
        /// @param force 
        void applyForce(const Vector2f& force);

        /// @brief get the axis alignement bounding box based on the shape
        /// @return return AABB
        AABB getAABB();

        private:
        Vector2f force_; // N
        Vector2f position_; // in meters = 1 pixel
        Vector2f linear_velocity_; // in m/s
        float angle_; // in radians
        float angular_velocity_; // radians/s

        AABB aabb_;
        std::vector<Vector2f> vertices_;
        std::vector<Vector2f> transformed_vertices_;
        bool transform_update_required_;
        bool aabb_update_required_;
        bool apply_gravity_;

        /// @brief create the vertices of a box; is used to tranform those points accordingly
        /// @param width 
        /// @param height 
        /// @return list of vertices
        static std::vector<Vector2f> createBoxVertices(float width, float height);

        // TODO: Possibility to add more convex shapes
    };
}

#endif