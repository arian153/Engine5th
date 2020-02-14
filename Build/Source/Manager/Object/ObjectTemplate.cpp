
namespace Engine5
{
    template <typename T>
    T* Object::AddComponent()
    {
        auto type = typeid(T).name();
        auto found = m_components.find(type);

        if (found == m_components.end())
        {
            auto created = m_component_manager->Create(type, this);
            m_components.emplace(type, created);

            return static_cast<T*>(created);
        }

        return nullptr;
    }

    template <typename T>
    bool Object::HasComponent() const
    {
        auto type = typeid(T).name();
        auto found = m_components.find(type);

        if (found != m_components.end())
            return true;

        return false;
    }

    template <typename T>
    T* Object::GetComponent() const
    {
        auto type = typeid(T).name();
        auto found = m_components.find(type);

        if (found != m_components.end())
            return static_cast<T*>(found->second);

        return nullptr;
    }

    template <typename T>
    void Object::RemoveComponent()
    {
        auto type = typeid(T).name();
        auto found = m_components.find(type);

        if (found != m_components.end())
        {
            m_component_manager->Remove(found->second, this);
            m_components.erase(found);
        }
    }
}