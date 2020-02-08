#pragma once
#include "../../Math/Math.hpp"

namespace Engine5
{
    class ColliderPrimitive;

    class ContactPoint
    {
    public:
        ContactPoint();
        ~ContactPoint();
        void          Clear();
        ContactPoint& operator=(const ContactPoint& rhs);
        bool          operator==(const ContactPoint& rhs) const;
        void          Swap();
        ContactPoint  SwappedContactPoint() const;
    public:
        //Two contact points in world space, each representing the deepest penetrating point of one collider.
        Vector3 global_position_a;
        Vector3 global_position_b;

        //Two contact points in local spaces of individual colliders, each corresponding to a contact point in world space. 
        //This information is crucial for maintaining persistent contacts. 
        Vector3 local_position_a;
        Vector3 local_position_b;

        //Contact normal representing the "best direction" to separate the two colliding colliders.
        Vector3 normal;

        //Two contact tangents perpendicular to each other and the contact normal. 
        //These vectors are used to resolve the frictional part of collision resolution.
        Vector3 tangent_a;
        Vector3 tangent_b;

        Vector3 c_a;
        Vector3 c_b;
        Vector3 r_a;
        Vector3 r_b;

        //Penetration depth, a scalar value that represents how deep the overlap of the two colliders.
        Real depth = 0.0f;

        //lagrangian multiplier sum.
        Real normal_impulse_sum    = 0.0f;
        Real tangent_a_impulse_sum = 0.0f;
        Real tangent_b_impulse_sum = 0.0f;

        Real normal_mass    = 0.0f;
        Real tangent_a_mass = 0.0f;
        Real tangent_b_mass = 0.0f;

        Real velocity_bias = 0.0f;

        ColliderPrimitive* collider_a = nullptr;
        ColliderPrimitive* collider_b = nullptr;

        bool b_valid      = true;
        bool b_persistent = false;
    };
}
