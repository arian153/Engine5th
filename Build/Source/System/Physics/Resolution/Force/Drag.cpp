#include "Drag.hpp"
#include "../../Dynamics/RigidBody.hpp"

namespace Engine5
{
    Drag::Drag()
    {
    }

    Drag::~Drag()
    {
    }

    void Drag::SetAngularDrag(Real drag)
    {
        angular_drag = drag;
    }

    void Drag::SetLinearDrag(Real drag)
    {
        linear_drag = drag;
    }

    void Drag::Update(RigidBody* body, Real dt)
    {
        if (body->GetMotionMode() == eMotionMode::Dynamic)
        {
            //Real    speed           = body->GetLinearVelocity().Length();
            //Real    an_speed        = body->GetAngularVelocity().Length();
            //Real    drag_multiplier = 6.f * Math::PI * air_viscosity * density * drag;
            //Vector3 drag_force      = -body->GetLinearVelocity().Normalize();
            //drag_force *= (drag_multiplier * speed);
            //Vector3 drag_torque = -body->GetAngularVelocity().Normalize();
            //drag_torque *= (drag_multiplier * an_speed);
            //body->ApplyForceCentroid(drag_force);
            //body->ApplyTorque(drag_torque);
            body->SetLinearVelocity(body->GetLinearVelocity() * powf(1.0f - linear_drag, dt));
            body->SetAngularVelocity(body->GetAngularVelocity() * powf(1.0f - angular_drag, dt));
        }
    }
}
