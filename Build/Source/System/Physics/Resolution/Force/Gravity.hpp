#pragma once
#include "Force.hpp"
#include "../../../Math/Math.hpp"

namespace Engine5
{
    class Gravity final : public Force
    {
    public:
        Gravity();
        ~Gravity();

        void Update(RigidBody* body, Real dt) override;
        void SetGlobalGravity(const Vector3& dir, Real magnitude);
        void SetLocalGravity(const Vector3& pos, Real mass, Real coefficient);

    private:
        Vector3 global_gravity = Vector3(0.0f, -1.0f, 0.0f);
        Vector3 local_position;
        Real    local_mass          = 1.0f;
        Real    gravity_coefficient = 1.0f;
        bool    b_global            = true;
    };
}
