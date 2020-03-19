#include "ObjectFactory.hpp"
#include "Object.hpp"
#include "ObjectManager.hpp"
#include "../Component/ComponentManager.hpp"

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
        m_archetype_component_manager->Initialize(component_registry);
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

    Object* ObjectFactory::CreateRawObject(const std::string& name, ObjectManager* object_manager)
    {
        Object* created = new Object();
        if (object_manager == nullptr)
        {
            created->SetName(name);
            return created;
        }
        else
        {
            object_manager->AddObject(name, created);
            return created;
        }
    }

    Object* ObjectFactory::CreateArchetypeObject(size_t archetype_id, ComponentManager* component_manager, const std::string& name, ObjectManager* object_manager)
    {
        if (m_archetypes.size() > archetype_id)
        {
            auto archetype = m_archetypes.at(archetype_id)->Clone(name, object_manager, component_manager);
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
