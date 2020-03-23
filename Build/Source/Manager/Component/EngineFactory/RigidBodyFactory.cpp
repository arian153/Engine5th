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
        return new RigidBodyComponent(owner);
    }

    Component* RigidBodyFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<RigidBodyComponent*>(origin);
        auto cloned = static_cast<RigidBodyComponent*>(this->Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
