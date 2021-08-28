#pragma once
#include "../ComponentFactory.hpp"

namespace Engine5
{
    class Component;
    class Object;

    class SkyFactory final : public ComponentFactory
    {
    public:
        SkyFactory();
        ~SkyFactory();

        Component* Create(Object* owner, Space* space) override;
        Component* Clone(Component* origin, Object* dest, Space* space) override;
    };
}
