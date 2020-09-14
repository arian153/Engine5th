#include "ObjectFactory.hpp"
#include "Object.hpp"
#include "ObjectManager.hpp"
#include "../Component/ComponentManager.hpp"
#include "../Resource/ResourceType/JsonResource.hpp"
#include "../../External/JSONCPP/json/json.h"
#include "../Component/Component.hpp"

namespace Engine5
{
    ObjectFactory::ObjectFactory()
    {
    }

    ObjectFactory::~ObjectFactory()
    {
    }

    void ObjectFactory::Initialize(ComponentRegistry* component_registry)
    {
        m_archetype_component_manager = new ComponentManager();
        m_archetype_component_manager->Initialize(component_registry, nullptr);
    }

    void ObjectFactory::Shutdown()
    {
        if (m_archetype_component_manager != nullptr)
        {
            m_archetype_component_manager->Shutdown();
            delete m_archetype_component_manager;
            m_archetype_component_manager = nullptr;
        }
    }

    Object* ObjectFactory::CreateRawObject(const std::string& name)
    {
        Object* created = new Object();
        created->m_name = name;
        return created;
    }

    Object* ObjectFactory::CreateRawObject(const std::string& name, ObjectManager* object_manager)
    {
        if (object_manager != nullptr)
        {
            return object_manager->AddObject(name);
        }
        return nullptr;
    }

    Object* ObjectFactory::CreateArchetypeObject(const std::string& name, size_t archetype_id, Space* space)
    {
        if (m_archetypes.size() > archetype_id)
        {
            auto archetype = m_archetypes.at(archetype_id)->Clone(name, space);
            return archetype;
        }
        return nullptr;
    }

    void ObjectFactory::AddArchetype(Object* object)
    {
        Object* archetype = CreateRawObject("", nullptr);
        object->CloneComponents(archetype, m_archetype_component_manager);
        if (object->m_children != nullptr)
        {
            object->CloneChildrenRecursive(archetype, this, m_archetype_component_manager);
        }
    }

    void ObjectFactory::AddArchetype(JsonResource* resource)
    {
        if (resource->IsArchetype() == true)
        {
            auto data = *resource->m_root_data;
            if (JsonResource::HasMember(data, "Name") && (data)["Name"].isString())
            {
                std::string name   = data["Name"].asString();
                Object*     object = CreateRawObject(name);
                object->Load(data, this);
            }
        }
    }

    size_t ObjectFactory::GetArchetypeID(JsonResource* resource)
    {
        auto found = m_resource_id.find(resource);
        if (found != m_resource_id.end())
        {
            return found->second;
        }
        return m_archetypes.size() + 1;
    }

    bool ObjectFactory::ValidateID(size_t archetype_id) const
    {
        return m_archetypes.size() > archetype_id;
    }

    size_t ObjectFactory::InvalidID() const
    {
        return m_archetypes.size() + 1;
    }

    void ObjectFactory::ClearArchetypes()
    {
        for (auto& archetype : m_archetypes)
        {
            archetype->ClearComponents();
            archetype->RemoveObjectHierarchy();
            delete archetype;
            archetype = nullptr;
        }
        m_archetypes.clear();
    }
}
