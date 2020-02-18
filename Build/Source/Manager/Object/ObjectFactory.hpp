#pragma once
#include <vector>
#include <string>

namespace Engine5
{
    class Object;
    class ObjectManager;

    class ObjectFactory
    {
    public:
        ObjectFactory();
        ~ObjectFactory();

        void Init();
        void Shutdown();

        Object* CreateRawObject(ObjectManager* object_manager = nullptr, const std::string& name = "Empty Object");
        Object* CreateArchetypeObject(size_t archetype_id, ObjectManager* object_manager = nullptr);

    private:
        void ClearArchetypes();

    private:
        std::vector<Object*> m_archetypes;
    };
}
