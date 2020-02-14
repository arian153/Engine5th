#pragma once
#include <unordered_map>

namespace Engine5
{
    class Object;
    class Component;

    class ComponentManager
    {
    public:
        ComponentManager();
        ~ComponentManager();

        void Initialize();
        void Shutdown();

        Component* Create(const std::string& type, Object* owner);
        Component* Find(const std::string& type, Object* owner);
        Component* Clone(Component* origin, Object* dest);

        void Find(const std::string& type, std::vector<Component*>& components);

        void Remove(Component* component);
        void Remove(Component* component, Object* owner);


    private:
        std::unordered_multimap<Object*, Component*> m_components;
    };
}
