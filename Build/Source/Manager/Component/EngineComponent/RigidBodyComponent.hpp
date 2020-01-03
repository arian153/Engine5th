#pragma once
#include "..//Component.hpp"
#include "../../../System/Physics/Physics.hpp"

namespace Engine5
{
    class RigidBodyComponent final : public Component
    {
    public:
        RigidBodyComponent();
        ~RigidBodyComponent();

    private:
        RigidBody* m_rigid_body = nullptr;
    };

}
