#include "ComponentManager.hpp"
#include "Component.hpp"

namespace Engine5
{
    ComponentManager::ComponentManager()
    {
    }

    ComponentManager::~ComponentManager()
    {
    }

    void ComponentManager::Initialize()
    {
    }

    void ComponentManager::Shutdown()
    {
    }

    Component* ComponentManager::Create(const std::string& type, Object* owner)
    {
    }

    Component* ComponentManager::Find(const std::string& type, Object* owner)
    {

    }

    Component* ComponentManager::Clone(Component* origin, Object* dest)
    {
    }

    void ComponentManager::Find(const std::string& type, std::vector<Component*>& components)
    {
        auto begin = m_components.begin();
        auto end   = m_components.end();
        for (auto it = begin; it != begin; ++it)
        {
            auto found = Find(type, it->first);
            if (found != nullptr)
            {
                components.push_back(found);
            }
        }
    }

    void ComponentManager::Remove(Component* component)
    {
        auto compo_ret = m_components.equal_range(component->m_owner);
        for (auto it = compo_ret.first; it != compo_ret.second; ++it)
        {
            if (it->second == component)
            {
                it->second->Shutdown();
                delete it->second;
                it->second = nullptr;
                m_components.erase(it);
                break;
            }
        }
    }

    void ComponentManager::Remove(Component* component, Object* owner)
    {
        if (component->m_owner == owner)
        {
            auto compo_ret = m_components.equal_range(owner);
            for (auto it = compo_ret.first; it != compo_ret.second; ++it)
            {
                if (it->second == component)
                {
                    it->second->Shutdown();
                    delete it->second;
                    it->second = nullptr;
                    m_components.erase(it);
                    break;
                }
            }
        }
    }
}
