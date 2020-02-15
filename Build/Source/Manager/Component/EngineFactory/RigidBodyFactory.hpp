#pragma once
#include "../ComponentFactory.hpp"

namespace Engine5
{
    class Component;
    class Object;

    class RigidBodyFactory final : public ComponentFactory
    {
    public:
        RigidBodyFactory();
        ~RigidBodyFactory();

        Component* Create(Object* owner) override;
        Component* Clone(Component* origin, Object* dest) override;
    };
}
