#include "../include/no_gravity_game_ws/momentOfInertia.hpp"

MomentOfInertia::MomentOfInertia()
    : moment_of_inertia_(1)
{
}

MomentOfInertia::MomentOfInertia(float x, float y, float z, float mass, std::string axis = "z")
{
    if (axis == "x")
        moment_of_inertia_ = 1.f/12.f * mass * (y*y + z*z);
    else if (axis == "y")
        moment_of_inertia_ = 1.f/12.f * mass * (x*x + z*z);
    else
        moment_of_inertia_ = 1.f/12.f * mass * (x*x + y*y); 
}

MomentOfInertia::~MomentOfInertia() = default;

float MomentOfInertia::getMomentOfInertia() const { return moment_of_inertia_; }

void MomentOfInertia::setMomentOfInertia(float MoI) { moment_of_inertia_ = MoI; }