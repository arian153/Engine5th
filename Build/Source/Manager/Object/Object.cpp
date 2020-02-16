#include "Object.hpp"
#include "../Component/ComponentManager.hpp"
#include "../Component/Component.hpp"

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

    void Object::RemoveChild(Object* child) const
    {
        if (m_children != nullptr)
        {
            auto it = std::find(m_children->begin(), m_children->end(), child);
            m_children->erase(it++);
        }
    }

    void Object::RemoveSibling(Object* sibling) const
    {
        if (m_children != nullptr)
        {
            auto it = std::find(m_sibling->begin(), m_sibling->end(), sibling);
            m_sibling->erase(it++);
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
            m_parent->RemoveChild(this);
        }
        if (m_sibling != nullptr)
        {
            for (auto sibling : *m_sibling)
            {
                sibling->RemoveSibling(this);
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
        if (m_parent != nullptr)
        {
            m_parent->RemoveChild(this);
        }
        if (m_sibling != nullptr)
        {
            for (auto& sibling : *m_sibling)
            {
                sibling->RemoveSibling(this);
            }
        }
        if (m_children != nullptr)
        {
            DestroyFamilyRecursive();
        }
    }

    void Object::DestroyFamilyRecursive()
    {
        if (m_children != nullptr)
        {
            for (auto& child : *m_children)
            {
                child->DestroyFamilyRecursive();
                child->m_parent   = nullptr;
                child->m_ancestor = nullptr;
                //m_object_manager->EraseObject(child);
                child->ClearComponents();
                delete child;
                child = nullptr;
            }
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

    void Object::ClearComponents()
    {
        m_component_manager->Remove(this);
        m_components.clear();
    }

    void Object::SetManager(ObjectManager* obj_manager, ComponentManager* cmp_manager)
    {
    }

    void Object::SetFactory(ObjectFactory* factory)
    {
    }

    Component* Object::AddComponent(Component* component)
    {
        auto type  = component->Type();
        auto found = m_components.find(type);
        if (found == m_components.end())
        {
            m_components.emplace(type, component);
            return component;
        }
        return nullptr;
    }

    Component* Object::AddComponent(const std::string& type)
    {
        auto found = m_components.find(type);
        if (found == m_components.end())
        {
            auto created = m_component_manager->Create(type, this);
            m_components.emplace(type, created);
            return created;
        }
        return nullptr;
    }

    Component* Object::GetComponent(const std::string& type)
    {
        auto found = m_components.find(type);
        if (found != m_components.end())
        {
            return found->second;
        }
        return nullptr;
    }

    bool Object::HasComponent(const std::string& type)
    {
        auto found = m_components.find(type);
        if (found != m_components.end())
        {
            return true;
        }
        return false;
    }

    void Object::RemoveComponent(Component* component)
    {
        auto type  = component->Type();
        auto found = m_components.find(type);
        if (found != m_components.end())
        {
            m_component_manager->Remove(component, this);
            m_components.erase(found);
        }
    }

    void Object::RemoveComponent(const std::string& type)
    {
        auto found = m_components.find(type);
        if (found != m_components.end())
        {
            m_component_manager->Remove(found->second, this);
            m_components.erase(found);
        }
    }
}
