#pragma once
#include <string>
#include <list>
#include <unordered_map>

namespace Engine5
{
    class ComponentFactory;

    class ComponentRegistry
    {
    public:
        ComponentRegistry();
        ~ComponentRegistry();

        bool RegisterFactories();
        bool AbolishFactories();
        bool AddFactory(const std::string& name, ComponentFactory* factory);

    private:
        std::unordered_map<std::string, ComponentFactory*> m_factories;
        std::list<std::string>                             m_keys;

        friend class ComponentManager;

    private:
    };
}
