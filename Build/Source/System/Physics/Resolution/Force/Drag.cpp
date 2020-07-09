#include "Drag.hpp"
#include "../../Dynamics/RigidBody.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

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

    void Drag::Initialize()
    {
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

    void Drag::Shutdown()
    {
    }

    void Drag::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Linear Drag") && data["Linear Drag"].isDouble())
        {
            linear_drag = data["Linear Drag"].asFloat();
        }
        if (JsonResource::HasMember(data, "Angular Drag") && data["Angular Drag"].isDouble())
        {
            angular_drag = data["Angular Drag"].asFloat();
        }
    }

    void Drag::Save(const Json::Value& data)
    {
    }
}
