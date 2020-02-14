#include "Component.hpp"

namespace Engine5
{
    Component::Component(Object* owner)
        : m_owner(owner)
    {
    }

    Component::~Component()
    {
    }

    String Component::Type() const
    {
        return m_type;
    }

    bool Component::IsLoaded() const
    {
        return m_b_loaded;
    }

    bool Component::IsUnloaded() const
    {
        return m_b_unloaded;
    }

    Object* Component::GetOwner() const
    {
        return m_owner;
    }
}
