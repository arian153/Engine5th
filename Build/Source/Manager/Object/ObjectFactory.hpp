#pragma once
#include <vector>
#include <string>

namespace Engine5
{
    class ComponentRegistry;
    class ComponentManager;
    class Object;
    class ObjectManager;

    class ObjectFactory
    {
    public:
        ObjectFactory();
        ~ObjectFactory();

        void Initialize(ComponentRegistry* component_registry);
        void Shutdown();

        Object* CreateRawObject(const std::string& name = "Empty Object", ObjectManager* object_manager = nullptr);
        Object* CreateArchetypeObject(size_t archetype_id, ComponentManager* component_manager, const std::string& name = "", ObjectManager* object_manager = nullptr);

        void AddArchetype(Object* object);

    private:
        void ClearArchetypes();

    private:
        std::vector<Object*> m_archetypes;
        ComponentManager*    m_archetype_component_manager = nullptr;
    };
}
