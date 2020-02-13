#include "ComponentManager.hpp"

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
    }

    void ComponentManager::Remove(Component* component, Object* object)
    {
    }
}
