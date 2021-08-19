#include "EnvironmentMapFactory.hpp"
#include "../EngineComponent/EnvironmentMapComponent.hpp"

namespace Engine5
{
    EnvironmentMapFactory::EnvironmentMapFactory()
    {
        type = typeid(EnvironmentMapComponent).name();
    }

    EnvironmentMapFactory::~EnvironmentMapFactory()
    {
    }

    Component* EnvironmentMapFactory::Create(Object* owner, Space* space)
    {
        auto create    = new EnvironmentMapComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* EnvironmentMapFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<EnvironmentMapComponent*>(origin);
        auto cloned = static_cast<EnvironmentMapComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
