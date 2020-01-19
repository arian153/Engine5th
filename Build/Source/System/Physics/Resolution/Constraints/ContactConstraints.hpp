#pragma once
#include "../../../Math/Math.hpp"

namespace Engine5
{
    class ContactPositionConstraints
    {
    public:
        ContactPositionConstraints();
        ~ContactPositionConstraints();

    public:
        Vector3  local_point_a;
        Vector3  local_point_b;
        Vector3  local_normal;
        Real     inv_mass_a;
        Real     inv_mass_b;
        Vector3  local_centroid_a;
        Vector3  local_centroid_b;
        Matrix33 inv_inertia_a;
        Matrix33 inv_inertia_b;
    };

    class ContactVelocityConstraints
    {
    public:
        ContactVelocityConstraints();
        ~ContactVelocityConstraints();

    public:
        Real restitution;
        Real friction;
    };
}
