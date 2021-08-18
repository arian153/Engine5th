#include "SkyBoxFactory.hpp"
#include "../EngineComponent/SkyBoxComponent.hpp"

namespace Engine5
{
    SkyBoxFactory::SkyBoxFactory()
    {
        type = typeid(SkyBoxComponent).name();
    }

    SkyBoxFactory::~SkyBoxFactory()
    {
    }

    Component* SkyBoxFactory::Create(Object* owner, Space* space)
    {
        auto create    = new SkyBoxComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* SkyBoxFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<SkyBoxComponent*>(origin);
        auto cloned = static_cast<SkyBoxComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
