#include "ComponentRegistry.hpp"
#include "ComponentFactory.hpp"

//factories
#include "EngineFactory/CameraFactory.hpp"
#include "EngineFactory/ColliderFactory.hpp"
#include "EngineFactory/RigidBodyFactory.hpp"
#include "EngineFactory/TransformFactory.hpp"

namespace Engine5
{
    ComponentRegistry::ComponentRegistry()
    {
    }

    ComponentRegistry::~ComponentRegistry()
    {
    }

    bool ComponentRegistry::RegisterFactories()
    {
        AddFactory(new TransformFactory());
        AddFactory(new RigidBodyFactory());
        AddFactory(new ColliderFactory());
        AddFactory(new CameraFactory());
        return true;
    }

    bool ComponentRegistry::AbolishFactories()
    {
        for (auto it = m_factories.begin(); it != m_factories.end(); ++it)
        {
            delete it->second;
            it->second = nullptr;
        }
        m_keys.clear();
        m_factories.clear();
        return true;
    }

    bool ComponentRegistry::AddFactory(ComponentFactory* factory)
    {
        if (factory != nullptr)
        {
            if (m_factories.find(factory->type) == m_factories.end())
            {
                m_keys.push_back(factory->type);
                m_factories.emplace(factory->type, factory);
                return true;
            }
        }
        return false;
    }
}
