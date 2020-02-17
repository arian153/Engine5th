#include "ObjectManager.hpp"
#include "Object.hpp"

namespace Engine5
{
    ObjectManager::ObjectManager()
    {
    }

    ObjectManager::~ObjectManager()
    {
    }

    Object* ObjectManager::AddObject(const std::string& name, Object* created)
    {
        Object* object = created;
        size_t  id     = m_objects.size();
        if (created == nullptr)
        {
            //object = m_object_factory->CreateRawObject(nullptr);
        }
        else
        {
            object->m_name = name;
            object->m_id   = id;
            object->SetManager(this);
            m_objects.push_back(object);
            m_object_map.emplace(name, object);
        }
        return object;
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
        auto range  = m_object_map.equal_range(name);
        auto result = range.first;
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

    void ObjectManager::RemoveObjects(const std::string& name)
    {
        auto ret = m_object_map.equal_range(name);
        for (auto it = ret.first; it != ret.second;)
        {
            m_objects.erase(std::find(m_objects.begin(), m_objects.end(), it->second));
            it->second->ClearComponents();
            delete it->second;
            it->second = nullptr;
            m_object_map.erase(it++);
        }
        ResetID();
    }

    void ObjectManager::RemoveObject(const std::string& name)
    {
        auto found = m_object_map.find(name);
        if (found != m_object_map.end())
        {
            m_objects.erase(std::find(m_objects.begin(), m_objects.end(), found->second));
            found->second->ClearComponents();
            delete found->second;
            found->second = nullptr;
            m_object_map.erase(found);
        }
        ResetID();
    }

    void ObjectManager::RemoveObjectAt(size_t id)
    {
        auto found = m_objects.at(id);
        m_objects.erase(m_objects.begin() + id);
        auto ret = m_object_map.equal_range(found->m_name);
        for (auto it = ret.first; it != ret.second; ++it)
        {
            if (it->second == found)
            {
                it->second->ClearComponents();
                delete it->second;
                it->second = nullptr;
                m_object_map.erase(it);
                break;
            }
        }
        ResetID();
    }

    void ObjectManager::RemoveObject(Object* object)
    {
        m_objects.erase(m_objects.begin() + object->m_id);
        auto ret = m_object_map.equal_range(object->m_name);
        for (auto it = ret.first; it != ret.second; ++it)
        {
            if (it->second == object)
            {
                object->ClearComponents();
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
        m_object_map.erase(std::find(ret.first, ret.second, object));
        m_objects.erase(std::find(m_objects.begin(), m_objects.end(), object));
        ResetID();
        return object;
    }

    Object* ObjectManager::EraseObjectAt(size_t id)
    {
        auto object = m_objects.at(id);
        m_objects.erase(m_objects.begin() + id);
        auto ret = m_object_map.equal_range(object->m_name);
        m_object_map.erase(std::find(ret.first, ret.second, object));
        ResetID();
        return object;
    }

    void ObjectManager::ClearObjects()
    {
    }

    bool ObjectManager::HasObject(const std::string& name)
    {
    }

    bool ObjectManager::HasObject(Object* object)
    {
    }

    void ObjectManager::ChangeName(Object* object, const std::string& new_name)
    {
    }

    void ObjectManager::ResetID()
    {
        size_t id = 0;
        for (auto& object : m_objects)
        {
            object->m_id = id++;
        }
    }
}
