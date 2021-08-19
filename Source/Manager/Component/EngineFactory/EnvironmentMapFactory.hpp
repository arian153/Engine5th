#pragma once
#include "../ComponentFactory.hpp"

namespace Engine5
{
    class Component;
    class Object;

    class EnvironmentMapFactory final : public ComponentFactory
    {
    public:
        EnvironmentMapFactory();
        ~EnvironmentMapFactory();

        Component* Create(Object* owner, Space* space) override;
        Component* Clone(Component* origin, Object* dest, Space* space) override;
    };
}
