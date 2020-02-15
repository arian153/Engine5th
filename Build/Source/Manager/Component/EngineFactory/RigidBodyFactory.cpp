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

    Component* RigidBodyFactory::Create(Object* owner)
    {
        return new RigidBodyComponent(owner);
    }

    Component* RigidBodyFactory::Clone(Component* origin, Object* dest)
    {
        auto source = static_cast<RigidBodyComponent*>(origin);
        auto cloned = static_cast<RigidBodyComponent*>(this->Create(dest));
        source->Clone(cloned);
        return cloned;
    }
}
