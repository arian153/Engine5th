#include "LightFactory.hpp"
#include "../EngineComponent/LightComponent.hpp"

namespace Engine5
{
    LightFactory::LightFactory()
    {
    }

    LightFactory::~LightFactory()
    {
    }

    Component* LightFactory::Create(Object* owner, Space* space)
    {
        auto create = new LightComponent(owner);
        create->SetSpace(space);
        return create;
    }

    Component* LightFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<LightComponent*>(origin);
        auto cloned = static_cast<LightComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
