#pragma once
#include "Force.hpp"
#include "../../../Math/Math.hpp"

namespace Engine5
{
    class Drag final : public Force
    {
    public:
        Drag();
        ~Drag();

        void SetAngularDrag(Real drag);
        void SetLinearDrag(Real drag);

        void Update(RigidBody* body, Real dt) override;


    private:
        Real linear_drag  = 0.25f;
        Real angular_drag = 0.25f;
    };
}
