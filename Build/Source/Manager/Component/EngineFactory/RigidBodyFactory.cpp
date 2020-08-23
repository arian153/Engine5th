#include "RigidBodyFactory.hpp"
#include "../EngineComponent/RigidBodyComponent.hpp"

namespace Engine5
{
    RigidBodyFactory::RigidBodyFactory()
    {
        type = typeid(RigidBodyComponent).name();
    }

    RigidBodyFactory::~RigidBodyFactory()
    {
    }

    Component* RigidBodyFactory::Create(Object* owner, Space* space)
    {
        auto create = new RigidBodyComponent(owner);
        create->m_type = type;
        create->SetSpace(space);
        return create;
    }

    Component* RigidBodyFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<RigidBodyComponent*>(origin);
        auto cloned = static_cast<RigidBodyComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
