#include "LogicComponent.hpp"

namespace Engine5
{
    LogicComponent::~LogicComponent()
    {
    }

    LogicComponent::LogicComponent(Object* owner)
        : Component(owner)
    {
    }
}
