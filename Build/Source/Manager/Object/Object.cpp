#include "Object.hpp"
#include "../Component/ComponentManager.hpp"
#include "../Component/Component.hpp"
#include "ObjectManager.hpp"
#include "ObjectFactory.hpp"
#include "../../System/Core/Utility/CoreUtility.hpp"
#include "../Resource/ResourceType/JsonResource.hpp"
#include "../../External/JSONCPP/json/json.h"
#include "../Space/Space.hpp"

namespace Engine5
{
    Object::Object()
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
        if (m_name != name)
        {
            if (m_object_manager != nullptr)
            {
                m_object_manager->ChangeName(this, name);
            }
            else
            {
                m_name = name;
            }
        }
    }

    Object* Object::Clone(const std::string& name, Space* space)
    {
        ObjectManager* obj         = space == nullptr ? m_object_manager : space->GetObjectManager();
        std::string    cloned_name = name == "" ? m_name : name;
        return obj->CloneObject(cloned_name, this);
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
        if (m_siblings != nullptr)
        {
            return m_siblings->at(index);
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
        return m_siblings;
    }

    void Object::AddChild(Object* baby_child)
    {
        if (this != baby_child)
        {
            if (m_children == nullptr)
            {
                m_children = new std::vector<Object*>();
            }
            if (baby_child->m_siblings == nullptr)
            {
                baby_child->m_siblings = new std::vector<Object*>();
            }
            for (auto& child : *m_children)
            {
                child->m_siblings->push_back(baby_child);
                baby_child->m_siblings->push_back(child);
            }
            m_children->push_back(baby_child);
            baby_child->m_parent   = this;
            baby_child->m_ancestor = m_ancestor == nullptr ? this : m_ancestor;
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
            auto it = std::find(m_siblings->begin(), m_siblings->end(), sibling);
            m_siblings->erase(it++);
        }
    }

    void Object::EraseObjectHierarchy()
    {
        if (m_parent != nullptr)
        {
            m_parent->RemoveChild(this);
        }
        if (m_siblings != nullptr)
        {
            for (auto sibling : *m_siblings)
            {
                sibling->RemoveSibling(this);
            }
            m_siblings->clear();
            delete m_siblings;
            m_siblings = nullptr;
        }
        if (m_children != nullptr)
        {
            for (auto& child : *m_children)
            {
                child->m_parent   = nullptr;
                child->m_ancestor = nullptr;
                if (child->m_siblings != nullptr)
                {
                    child->m_siblings->clear();
                    delete child->m_siblings;
                    child->m_siblings = nullptr;
                }
            }
            m_children->clear();
            delete m_children;
            m_children = nullptr;
        }
    }

    void Object::RemoveObjectHierarchy()
    {
        if (m_parent != nullptr)
        {
            m_parent->RemoveChild(this);
        }
        if (m_siblings != nullptr)
        {
            for (auto& sibling : *m_siblings)
            {
                sibling->RemoveSibling(this);
            }
        }
        if (m_children != nullptr)
        {
            RemoveChildrenRecursive();
        }
    }

    void Object::ClearComponents()
    {
        m_component_manager->Remove(this);
        m_components.clear();
    }

    bool Object::Load(const Json::Value& data)
    {
        //Add Components
        if (JsonResource::HasMember(data, "Components") && data["Components"].isArray())
        {
            for (auto it = data["Components"].begin(); it != data["Components"].end(); ++it)
            {
                //Load Components
                if (JsonResource::HasMember(*it, "Type") && (*it)["Type"].isString())
                {
                    std::string type    = "class Engine5::" + (*it)["Type"].asString();
                    Component*  created = AddComponent(type);
                    if (created != nullptr)
                    {
                        created->Load((*it)["Value"]);
                    }
                }
            }
        }
        //Add Children Objects
        if (JsonResource::HasMember(data, "Children") && data["Children"].isArray())
        {
            for (auto it = data["Children"].begin(); it != data["Children"].end(); ++it)
            {
                //Load Child
                if (JsonResource::HasMember(*it, "Name") && (*it)["Name"].isString())
                {
                    auto child = m_object_manager->AddObject((*it)["Name"].asString());
                    child->Load(*it);
                    AddChild(child);
                }
            }
        }
        return true;
    }

    bool Object::Save(Json::Value& data) const
    {
        data["Name"] = m_name;
        return true;
    }

    void Object::SetManager(ObjectManager* object_manager)
    {
        m_object_manager = object_manager;
    }

    void Object::SetManager(ComponentManager* component_manager)
    {
        m_component_manager = component_manager;
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
            component->Initialize();
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
            created->Initialize();
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

    void Object::ClearObjectHierarchy()
    {
        if (m_children != nullptr)
        {
            m_children->clear();
            delete m_children;
            m_children = nullptr;
        }
        if (m_siblings != nullptr)
        {
            m_siblings->clear();
            delete m_siblings;
            m_siblings = nullptr;
        }
    }

    void Object::RemoveChildrenRecursive()
    {
        if (m_children != nullptr)
        {
            for (auto& child : *m_children)
            {
                child->RemoveChildrenRecursive();
                child->m_parent   = nullptr;
                child->m_ancestor = nullptr;
                if (m_object_manager != nullptr)
                {
                    m_object_manager->EraseObject(child, false);
                }
                child->ClearComponents();
                delete child;
                child = nullptr;
            }
            m_children->clear();
            delete m_children;
            m_children = nullptr;
        }
        if (m_siblings != nullptr)
        {
            m_siblings->clear();
            delete m_siblings;
            m_siblings = nullptr;
        }
    }

    bool Object::CloneComponents(Object* cloned_object, ComponentManager* manager) const
    {
        for (auto& compo : m_components)
        {
            auto cloned_compo = manager->Clone(compo.second, cloned_object);
            auto result       = cloned_object->AddComponent(cloned_compo);
            E5_ASSERT(result == nullptr, "ERROR : Clone " + compo.first + " Failed!");
        }
        return true;
    }

    bool Object::CloneHierarchy(Object* cloned_object, ObjectManager* obj, ComponentManager* cmp) const
    {
        if (obj == m_object_manager)
        {
            if (m_parent != nullptr)
            {
                m_parent->AddChild(cloned_object);
            }
        }
        if (m_children != nullptr)
        {
            //copy recursive.
            CloneChildrenRecursive(cloned_object, obj, cmp);
        }
        return true;
    }

    void Object::CloneChildrenRecursive(Object* cloned_object, ObjectManager* obj, ComponentManager* cmp) const
    {
        if (m_children != nullptr)
        {
            for (auto& child : *m_children)
            {
                Object* cloned_child = obj->AddObject(child->m_name);
                child->CloneComponents(cloned_child, cmp);
                cloned_object->AddChild(cloned_child);
                child->CloneChildrenRecursive(cloned_child, obj, cmp);
            }
        }
    }

    void Object::CloneChildrenRecursive(Object* cloned_object, ObjectFactory* obj, ComponentManager* cmp) const
    {
        if (m_children != nullptr)
        {
            for (auto& child : *m_children)
            {
                Object* cloned_child = obj->CreateRawObject(child->m_name, nullptr);
                child->CloneComponents(cloned_child, cmp);
                cloned_object->AddChild(cloned_child);
                child->CloneChildrenRecursive(cloned_child, obj, cmp);
            }
        }
    }

    bool Object::Load(const Json::Value& data, ObjectFactory* obj_factory)
    {
        //Add Components
        if (JsonResource::HasMember(data, "Components") && data["Components"].isArray())
        {
            for (auto it = data["Components"].begin(); it != data["Components"].end(); ++it)
            {
                //Load Components
                if (JsonResource::HasMember(*it, "Type") && (*it)["Type"].isString())
                {
                    auto created = obj_factory->m_archetype_component_manager->Create((*it)["Type"].asString(), this);
                    if (created != nullptr)
                    {
                        created->Load((*it)["Value"]);
                    }
                }
            }
        }
        //Add Children Objects
        if (JsonResource::HasMember(data, "Children") && data["Children"].isArray())
        {
            for (auto it = data["Children"].begin(); it != data["Children"].end(); ++it)
            {
                //Load Child
                if (JsonResource::HasMember(*it, "Name") && (*it)["Name"].isString())
                {
                    auto child = obj_factory->CreateRawObject((*it)["Name"].asString());
                    child->Load(*it, obj_factory);
                    this->AddChild(child);
                }
            }
        }
        return true;
    }
}
