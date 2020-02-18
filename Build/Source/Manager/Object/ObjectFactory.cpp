#include "ObjectFactory.hpp"
#include "Object.hpp"
#include "ObjectManager.hpp"

namespace Engine5
{
    ObjectFactory::ObjectFactory()
    {
    }

    ObjectFactory::~ObjectFactory()
    {
    }

    void ObjectFactory::Init()
    {
    }

    void ObjectFactory::Shutdown()
    {
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

    Object* ObjectFactory::CreateArchetypeObject(size_t archetype_id, const std::string& name, ObjectManager* object_manager)
    {
        if (m_archetypes.size() > archetype_id)
        {
            auto archetype = m_archetypes.at(archetype_id)->Clone(name, object_manager);
            return archetype;
        }
        return nullptr;
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
