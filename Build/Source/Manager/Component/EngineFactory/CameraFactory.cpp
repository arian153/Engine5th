#include "CameraFactory.hpp"
#include "../EngineComponent/CameraComponent.hpp"

namespace Engine5
{
    CameraFactory::CameraFactory()
    {
        type = typeid(CameraComponent).name();
    }

    CameraFactory::~CameraFactory()
    {
    }

    Component* CameraFactory::Create(Object* owner)
    {
        return new CameraComponent(owner);
    }

    Component* CameraFactory::Clone(Component* origin, Object* dest)
    {
        auto source = static_cast<CameraComponent*>(origin);
        auto cloned = static_cast<CameraComponent*>(this->Create(dest));
        source->Clone(cloned);
        return cloned;
    }
}
