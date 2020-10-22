#include "Gravity.hpp"
#include "../../Dynamics/RigidBody.hpp"
#include "../../../../Manager/Resource/ResourceType/JsonResource.hpp"
#include "../../../../External/JSONCPP/json/json.h"

namespace Engine5
{
    GravityFactory::GravityFactory()
    {
        type = typeid(Gravity).name();
    }

    GravityFactory::~GravityFactory()
    {
    }

    Force* GravityFactory::Create()
    {
        auto created = new Gravity();
        return created;
    }

    Gravity::Gravity()
    {
        type = typeid(Gravity).name();
    }

    Gravity::~Gravity()
    {
    }

    void Gravity::Initialize()
    {
    }

    void Gravity::Update(RigidBody* body, Real dt)
    {
        if (body->GetMotionMode() == eMotionMode::Dynamic)
        {
            if (b_global)
            {
                body->ApplyForceCentroid(global_gravity * body->Mass());
            }
            else
            {
                Vector3 distance = local_position - body->GetCentroid();
                Real    r_r      = distance.LengthSquared();
                Real    force    = r_r > 1.0f ? gravity_coefficient * local_mass * body->Mass() * dt / r_r : 0.0f;
                body->ApplyForceCentroid(force * distance.Normalize());
            }
        }
    }

    void Gravity::Shutdown()
    {
    }

    void Gravity::Load(const Json::Value& data)
    {
        if (JsonResource::HasMember(data, "Gravity Coefficient") && data["Gravity Coefficient"].isDouble())
        {
            gravity_coefficient = data["Gravity Coefficient"].asFloat();
        }
        if (JsonResource::HasMember(data, "Local Mass") && data["Local Mass"].isDouble())
        {
            local_mass = data["Local Mass"].asFloat();
        }
        if (JsonResource::HasMember(data, "Local Position") && JsonResource::IsVector3(data["Local Position"]))
        {
            local_position = JsonResource::AsVector3(data["Local Position"]);
        }
        if (JsonResource::HasMember(data, "Global Gravity") && JsonResource::IsVector3(data["Global Gravity"]))
        {
            global_gravity = JsonResource::AsVector3(data["Global Gravity"]);
        }
    }

    void Gravity::Save(const Json::Value& data)
    {
    }

    void Gravity::SetGlobalGravity(const Vector3& dir, Real magnitude)
    {
        global_gravity = dir.Normalize() * magnitude;
        b_global       = true;
    }

    void Gravity::SetLocalGravity(const Vector3& pos, Real mass, Real coefficient)
    {
        local_position      = pos;
        local_mass          = mass;
        gravity_coefficient = coefficient;
        b_global            = false;
    }
}
