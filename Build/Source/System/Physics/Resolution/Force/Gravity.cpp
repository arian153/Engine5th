#include "Gravity.hpp"
#include "../../Dynamics/RigidBody.hpp"

namespace Engine5
{
    Gravity::Gravity()
    {
    }

    Gravity::~Gravity()
    {
    }

    void Gravity::Update(RigidBody* body, Real dt)
    {
        if (body->GetMotionMode() == eMotionMode::Dynamic)
        {
            if (b_global)
            {
                body->AddLinearVelocity(global_gravity);
            }
            else
            {
                Vector3 distance = local_position - body->GetCentroid();
                Real    r_r      = distance.LengthSquared();
                Real    force    = r_r > Math::EPSILON ? gravity_coefficient * local_mass * dt / r_r : 0.0f;
                body->AddLinearVelocity(force * distance.Normalize());
            }
        }
    }

    void Gravity::SetGlobalGravity(const Vector3& dir, Real magnitude)
    {
        global_gravity = dir.Normalize() * magnitude;
    }

    void Gravity::SetLocalGravity(const Vector3& pos, Real mass, Real coefficient)
    {
        local_position      = pos;
        local_mass          = mass;
        gravity_coefficient = coefficient;
    }
}
