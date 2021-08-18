#pragma once
#include "../ComponentFactory.hpp"

namespace Engine5
{
    class Component;
    class Object;

    class SkyBoxFactory final : public ComponentFactory
    {
    public:
        SkyBoxFactory();
        ~SkyBoxFactory();

        Component* Create(Object* owner, Space* space) override;
        Component* Clone(Component* origin, Object* dest, Space* space) override;
    };
}
