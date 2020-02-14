#include "ComponentRegistry.hpp"

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
        return true;
    }

    bool ComponentRegistry::AbolishFactories()
    {
        for (auto it = m_factories.begin(); it != m_factories.end(); ++it)
        {
            //delete it->second;
            it->second = nullptr;
        }
        m_keys.clear();
        m_factories.clear();
        return true;
    }

    bool ComponentRegistry::AddFactory(const std::string& name, ComponentFactory* factory)
    {
        if (m_factories.find(name) == m_factories.end())
        {
            m_keys.push_back(name);
            m_factories.emplace(name, factory);
            return true;
        }
        return false;
    }
}
