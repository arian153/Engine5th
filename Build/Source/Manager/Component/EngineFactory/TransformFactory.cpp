#include "TransformFactory.hpp"
#include "../EngineComponent/TransformComponent.hpp"

namespace Engine5
{
    TransformFactory::TransformFactory()
    {
        type = typeid(TransformComponent).name();
    }

    TransformFactory::~TransformFactory()
    {
    }

    Component* TransformFactory::Create(Object* owner)
    {
        return new TransformComponent(owner);
    }

    Component* TransformFactory::Clone(Component* origin, Object* dest)
    {
        auto source = static_cast<TransformComponent*>(origin);
        auto cloned = static_cast<TransformComponent*>(this->Create(dest));
        source->Clone(cloned);
        return cloned;
    }
}
