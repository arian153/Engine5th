#include "Object.hpp"

namespace Engine5
{
    Object::Object()
        : m_id(s_id++)
    {
    }

    Object::~Object()
    {
    }

    Object* Object::GetChildAt(size_t index) const
    {
        if (m_children != nullptr)
        {
            return m_children->at(index);
        }
        return nullptr;
    }

    Object* Object::GetSiblingAt(size_t index) const
    {
        if (m_sibling != nullptr)
        {
            return m_sibling->at(index);
        }
        return nullptr;
    }

    Object* Object::GetParent() const
    {
        return m_parent;
    }

    Object* Object::GetAncestor() const
    {
        return m_ancestor;
    }
}
