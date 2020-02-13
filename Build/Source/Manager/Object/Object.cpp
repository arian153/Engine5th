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

    size_t Object::GetID() const
    {
        return m_id;
    }

    std::string Object::GetName() const
    {
        return m_name;
    }

    void Object::SetName(const std::string& name)
    {
    }

    Object* Object::Clone(ObjectManager* manager)
    {
        return nullptr;
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

    std::vector<Object*>* Object::GetChildren() const
    {
        return m_children;
    }

    std::vector<Object*>* Object::GetSibling() const
    {
        return m_sibling;
    }

    void Object::AddChild(Object* baby_child)
    {
        if (this != baby_child)
        {
            if (m_children == nullptr)
            {
                m_children = new std::vector<Object*>();
            }
            if (baby_child->m_sibling == nullptr)
            {
                baby_child->m_sibling = new std::vector<Object*>();
            }
            if (m_children->empty() == false)
            {
                for (auto& child : *m_children)
                {
                    child->m_sibling->push_back(baby_child);
                    baby_child->m_sibling->push_back(child);
                }
            }
            m_children->push_back(baby_child);
            baby_child->m_parent = this;
            if (this->m_ancestor == nullptr)
            {
                baby_child->m_ancestor = this;
            }
            else
            {
                baby_child->m_ancestor = this->m_ancestor;
            }
        }
    }

    void Object::ClearObjectHierarchy()
    {
        if (m_children != nullptr)
        {
            m_children->clear();
            delete m_children;
            m_children = nullptr;
        }
        if (m_sibling != nullptr)
        {
            m_sibling->clear();
            delete m_sibling;
            m_sibling = nullptr;
        }
    }

    void Object::ReleaseObjectHierarchy()
    {
        if (m_parent != nullptr)
        {
            auto it = std::find(m_parent->m_children->begin(), m_parent->m_children->end(), this);
            m_parent->m_children->erase(it++);
        }
        if (m_sibling != nullptr)
        {
            for (auto brother : *m_sibling)
            {
                auto it = std::find(brother->m_sibling->begin(), brother->m_sibling->end(), this);
                brother->m_sibling->erase(it++);
            }
        }
        if (m_children != nullptr)
        {
            for (auto& child : *m_children)
            {
                child->m_parent   = nullptr;
                child->m_ancestor = nullptr;
                if (child->m_sibling != nullptr)
                {
                    child->m_sibling->clear();
                    delete child->m_sibling;
                    child->m_sibling = nullptr;
                }
            }
            m_children->clear();
            delete m_children;
            m_children = nullptr;
        }
    }

    void Object::DestroyObjectHierarchy()
    {
    }

    void Object::InheritObjectHierarchy()
    {
    }

    void Object::ClearComponents()
    {
    }
}
