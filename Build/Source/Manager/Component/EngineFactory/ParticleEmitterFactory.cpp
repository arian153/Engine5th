#include "ParticleEmitterFactory.hpp"
#include "../EngineComponent/ParticleEmitterComponent.hpp"

namespace Engine5
{
    ParticleEmitterFactory::ParticleEmitterFactory()
    {
        type = typeid(ParticleEmitterComponent).name();
    }

    ParticleEmitterFactory::~ParticleEmitterFactory()
    {
    }

    Component* ParticleEmitterFactory::Create(Object* owner, Space* space)
    {
        auto create = new ParticleEmitterComponent(owner);
        create->SetSpace(space);
        return create;
    }

    Component* ParticleEmitterFactory::Clone(Component* origin, Object* dest, Space* space)
    {
        auto source = static_cast<ParticleEmitterComponent*>(origin);
        auto cloned = static_cast<ParticleEmitterComponent*>(Create(dest, space));
        cloned->Clone(source);
        return cloned;
    }
}
