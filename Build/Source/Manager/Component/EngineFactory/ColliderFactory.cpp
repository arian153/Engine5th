#include "ColliderFactory.hpp"
#include "../EngineComponent/ColliderComponent.hpp"

namespace Engine5
{
    ColliderFactory::ColliderFactory()
    {
        type = typeid(ColliderComponent).name();
    }

    ColliderFactory::~ColliderFactory()
    {
    }

    Component* ColliderFactory::Create(Object* owner)
    {
        return new ColliderComponent(owner);
    }

    Component* ColliderFactory::Clone(Component* origin, Object* dest)
    {
        auto source = static_cast<ColliderComponent*>(origin);
        auto cloned = static_cast<ColliderComponent*>(this->Create(dest));
        cloned->Clone(source);
        return cloned;
    }
}
