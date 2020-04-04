#pragma once
#include "../ComponentFactory.hpp"

namespace Engine5
{
    class Component;
    class Object;

    class MeshFactory final : public ComponentFactory
    {
    public:
        MeshFactory();
        ~MeshFactory();

        Component* Create(Object* owner, Space* space) override;
        Component* Clone(Component* origin, Object* dest, Space* space) override;
    };
}
