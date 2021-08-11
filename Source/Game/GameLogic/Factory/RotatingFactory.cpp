#include "RotatingFactory.h"
#include "../../../Manager/Component/Component.hpp"
#include "../Component/RotatingComponent.h"

namespace Game
{
    RotatingFactory::RotatingFactory()
    {
        type = typeid(RotatingComponent).name();
    }

    RotatingFactory::~RotatingFactory()
    {
    }

    Engine5::Component* RotatingFactory::Create(Engine5::Object* owner, Engine5::Space* space)
    {
        auto create = new RotatingComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Engine5::Component* RotatingFactory::Clone(Engine5::Component* origin, Engine5::Object* dest, Engine5::Space* space)
    {
        auto source = static_cast<RotatingComponent*>(origin);
        auto cloned = static_cast<RotatingComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
