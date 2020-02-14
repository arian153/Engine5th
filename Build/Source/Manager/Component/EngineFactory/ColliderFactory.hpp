#pragma once
#include "../ComponentFactory.hpp"

namespace Engine5
{
    class Component;
    class Object;

    class ColliderFactory final : public ComponentFactory
    {
        ColliderFactory();
        ~ColliderFactory();

        Component* Create(Object* owner) override;
        Component* Clone(Component* origin, Object* dest) override;
    };
}
