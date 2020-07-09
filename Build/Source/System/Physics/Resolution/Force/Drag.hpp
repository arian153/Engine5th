#pragma once
#include "Force.hpp"
#include "../../../Math/Math.hpp"

namespace Json
{
    class Value;
}

namespace Engine5
{
    class Drag final : public Force
    {
    public:
        Drag();
        ~Drag();

        void SetAngularDrag(Real drag);
        void SetLinearDrag(Real drag);

        void Initialize() override;
        void Update(RigidBody* body, Real dt) override;
        void Shutdown() override;
        void Load(const Json::Value& data) override;
        void Save(const Json::Value& data) override;

    private:
        Real linear_drag  = 0.25f;
        Real angular_drag = 0.25f;
    };
}
