#include "ControllerFactory.h"
#include "../../../Manager/Component/Component.hpp"
#include "../Component/ControllerComponent.h"

namespace Game
{
    ControllerFactory::ControllerFactory()
    {
        type = typeid(ControllerComponent).name();
    }

    ControllerFactory::~ControllerFactory()
    {
    }

    Engine5::Component* ControllerFactory::Create(Engine5::Object* owner, Engine5::Space* space)
    {
        auto create    = new ControllerComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Engine5::Component* ControllerFactory::Clone(Engine5::Component* origin, Engine5::Object* dest, Engine5::Space* space)
    {
        auto source = static_cast<ControllerComponent*>(origin);
        auto cloned = static_cast<ControllerComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
