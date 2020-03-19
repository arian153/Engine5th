#include "ObjectManager.hpp"
#include "Object.hpp"
#include "ObjectFactory.hpp"
#include "../Component/ComponentManager.hpp"

namespace Engine5
{
    ObjectManager::ObjectManager()
    {
    }

    ObjectManager::~ObjectManager()
    {
    }

    void ObjectManager::Initialize(ObjectFactory* object_factory)
    {
        m_object_factory = object_factory;
    }

    void ObjectManager::Shutdown()
    {
        for (auto& object : m_objects)
        {
            object->m_components.clear();
            object->ClearObjectHierarchy();
            delete object;
            object = nullptr;
        }
        m_objects.clear();
        m_object_map.clear();
    }

    Object* ObjectManager::AddObject(const std::string& name, Object* created)
    {
        Object* object = created;
        size_t  id     = m_objects.size();
        if (created == nullptr)
        {
            object = m_object_factory->CreateRawObject(name, nullptr);
        }
        object->m_name = name;
        object->m_id   = id;
        object->SetManager(this);
        m_objects.push_back(object);
        m_object_map.emplace(name, object);
        return object;
    }

    Object* ObjectManager::AddObject(const std::string& name, size_t archetype_id, ComponentManager* cmp_m)
    {
        return m_object_factory->CreateArchetypeObject(archetype_id, cmp_m, name, this);
    }

    Object* ObjectManager::CloneObject(const std::string& name, Object* origin, ComponentManager* cmp_m)
    {
        Object* cloned_object = AddObject(name);
        auto    cmp           = cmp_m == nullptr ? origin->m_component_manager : cmp_m;
        origin->CloneComponents(cloned_object, cmp);
        origin->CloneHierarchy(cloned_object, this, cmp);
        return cloned_object;
    }

    void ObjectManager::FindObjects(const std::string& name, std::vector<Object*>& objects)
    {
        auto ret = m_object_map.equal_range(name);
        for (auto founds = ret.first; founds != ret.second; ++founds)
        {
            objects.push_back(founds->second);
        }
    }

    Object* ObjectManager::FindObjectBegin(const std::string& name)
    {
        auto ret    = m_object_map.equal_range(name);
        auto result = ret.first;
        if (result != m_object_map.end())
        {
            return result->second;
        }
        return nullptr;
    }

    Object* ObjectManager::FindObjectEnd(const std::string& name)
    {
        auto ret = m_object_map.equal_range(name);
        if (ret.first != ret.second)
        {
            auto it     = std::distance(ret.first, ret.second);
            auto result = ret.first;
            std::advance(result, it - 1);
            return result->second;
        }
        return nullptr;
    }

    Object* ObjectManager::FindObjectAt(size_t id)
    {
        return m_objects.at(id);
    }

    void ObjectManager::RemoveObjects(const std::string& name, bool b_remove_hierarchy)
    {
        std::vector<Object*> removals;
        auto                 ret = m_object_map.equal_range(name);
        for (auto it = ret.first; it != ret.second;)
        {
            removals.push_back(it->second);
            m_objects.erase(std::find(m_objects.begin(), m_objects.end(), it->second));
            m_object_map.erase(it++);
        }
        for (auto& removal : removals)
        {
            if (removal != nullptr)
            {
                removal->ClearComponents();
                b_remove_hierarchy ? removal->RemoveObjectHierarchy() : removal->EraseObjectHierarchy();
                delete removal;
                removal = nullptr;
            }
        }
        removals.clear();
        ResetID();
    }

    void ObjectManager::RemoveObject(const std::string& name, bool b_remove_hierarchy)
    {
        auto found = m_object_map.find(name);
        if (found != m_object_map.end())
        {
            m_objects.erase(std::find(m_objects.begin(), m_objects.end(), found->second));
            found->second->ClearComponents();
            b_remove_hierarchy ? found->second->RemoveObjectHierarchy() : found->second->EraseObjectHierarchy();
            delete found->second;
            found->second = nullptr;
            m_object_map.erase(found);
        }
        ResetID();
    }

    void ObjectManager::RemoveObjectAt(size_t id, bool b_remove_hierarchy)
    {
        auto found = m_objects.at(id);
        m_objects.erase(m_objects.begin() + id);
        auto ret = m_object_map.equal_range(found->m_name);
        for (auto it = ret.first; it != ret.second; ++it)
        {
            if (it->second == found)
            {
                it->second->ClearComponents();
                b_remove_hierarchy ? it->second->RemoveObjectHierarchy() : it->second->EraseObjectHierarchy();
                delete it->second;
                it->second = nullptr;
                m_object_map.erase(it);
                break;
            }
        }
        ResetID();
    }

    void ObjectManager::RemoveObject(Object* object, bool b_remove_hierarchy)
    {
        m_objects.erase(m_objects.begin() + object->m_id);
        auto ret = m_object_map.equal_range(object->m_name);
        for (auto it = ret.first; it != ret.second; ++it)
        {
            if (it->second == object)
            {
                object->ClearComponents();
                b_remove_hierarchy ? object->RemoveObjectHierarchy() : object->EraseObjectHierarchy();
                delete object;
                object = nullptr;
                m_object_map.erase(it);
                break;
            }
        }
        ResetID();
    }

    Object* ObjectManager::EraseObject(Object* object)
    {
        auto ret = m_object_map.equal_range(object->m_name);
        for (auto it = ret.first; it != ret.second; ++it)
        {
            if (it->second == object)
            {
                m_object_map.erase(it);
                break;
            }
        }
        m_objects.erase(std::find(m_objects.begin(), m_objects.end(), object));
        object->EraseObjectHierarchy();
        ResetID();
        return object;
    }

    Object* ObjectManager::EraseObjectAt(size_t id)
    {
        auto object = m_objects.at(id);
        m_objects.erase(m_objects.begin() + id);
        auto ret = m_object_map.equal_range(object->m_name);
        for (auto it = ret.first; it != ret.second; ++it)
        {
            if (it->second == object)
            {
                m_object_map.erase(it);
                break;
            }
        }
        object->EraseObjectHierarchy();
        ResetID();
        return object;
    }

    void ObjectManager::ClearObjects()
    {
        for (auto& object : m_objects)
        {
            object->ClearComponents();
            object->ClearObjectHierarchy();
            delete object;
            object = nullptr;
        }
        m_objects.clear();
        m_object_map.clear();
    }

    bool ObjectManager::HasObject(const std::string& name)
    {
        auto result = m_object_map.find(name);
        if (result != m_object_map.end())
        {
            return true;
        }
        return false;
    }

    bool ObjectManager::HasObject(Object* object)
    {
        auto ret = m_object_map.equal_range(object->m_name);
        for (auto it = ret.first; it != ret.second; ++it)
        {
            if (it->second == object)
            {
                return true;
            }
        }
        return false;
    }

    void ObjectManager::ChangeName(Object* object, const std::string& new_name)
    {
        auto ret = m_object_map.equal_range(object->m_name);
        if (ret.first != ret.second)
        {
            //m_object_map.erase(std::find(ret.first, ret.second, object));
            m_object_map.emplace(new_name, object);
            object->m_name = new_name;
        }
    }

    void ObjectManager::ResetID()
    {
        size_t id = 0;
        for (auto& object : m_objects)
        {
            object->m_id = id++;
        }
    }

    void ObjectManager::EraseObject(Object* object, bool b_erase_hierarchy)
    {
        auto ret = m_object_map.equal_range(object->m_name);
        for (auto it = ret.first; it != ret.second; ++it)
        {
            if (it->second == object)
            {
                m_object_map.erase(it);
                break;
            }
        }
        m_objects.erase(std::find(m_objects.begin(), m_objects.end(), object));
        if (b_erase_hierarchy == true)
        {
            object->EraseObjectHierarchy();
        }
        ResetID();
    }
}
