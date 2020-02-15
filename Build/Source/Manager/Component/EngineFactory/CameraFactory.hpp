#pragma once
#include "../ComponentFactory.hpp"

namespace Engine5
{
    class Component;
    class Object;

    class CameraFactory final : public ComponentFactory
    {
    public:
        CameraFactory();
        ~CameraFactory();

        Component* Create(Object* owner) override;
        Component* Clone(Component* origin, Object* dest) override;
    };
}
