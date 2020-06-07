#pragma once
#include "../../../Manager/Component/ComponentFactory.hpp"

namespace Game
{
    class ControllerFactory final : public Engine5::ComponentFactory
    {
    public:
        ControllerFactory();
        ~ControllerFactory();

        Engine5::Component* Create(Engine5::Object* owner, Engine5::Space* space) override;
        Engine5::Component* Clone(Engine5::Component* origin, Engine5::Object* dest, Engine5::Space* space) override;
    };
}
