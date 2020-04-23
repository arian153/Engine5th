#pragma once
#include "../ComponentFactory.hpp"

namespace Engine5
{
    class Component;
    class Object;

    class TextSpriteFactory final : public ComponentFactory
    {
    public:
        TextSpriteFactory();
        ~TextSpriteFactory();

        Component* Create(Object* owner, Space* space) override;
        Component* Clone(Component* origin, Object* dest, Space* space) override;
    };
}
