#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class MomentOfInertia
{
    public:
    MomentOfInertia();
    MomentOfInertia(float x, float y, float z, float mass, std::string axis); // Rectangular cube
    ~MomentOfInertia();

    float getMomentOfInertia() const;
    void setMomentOfInertia(float MoI);

    private:
    float moment_of_inertia_;
};